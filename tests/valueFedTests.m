function tests=valueFedTests

tests=functiontests(localfunctions);
end

function setup(testCase)  % do not change function name
% open a figure, for example

end

function teardown(testCase)  % do not change function name
% close figure, for example
end


function [fedStruct,success]=generateFed()
import helics.*
 success=true;
initstring = '-f1';
fedinitstring = '--broker=mainbroker --federates=1';
fedStruct.broker=helics.helicsCreateBroker('zmq','mainbroker',initstring);
if (~helics.helicsBrokerIsValid(fedStruct.broker))
    success=false;
    return;
end
fedInfo=helics.helicsCreateFederateInfo();
if (fedInfo==0)
    success=false;
    return;
end
try
helics.helicsFederateInfoSetCoreTypeFromString(fedInfo,'zmq');
helics.helicsFederateInfoSetCoreInitString(fedInfo,fedinitstring);
helics.helicsFederateInfoSetTimeProperty(fedInfo,helics.HelicsProperties.HELICS_PROPERTY_TIME_DELTA, 0.01);
helics.helicsFederateInfoSetIntegerProperty(fedInfo,helics.HelicsProperties.HELICS_PROPERTY_INT_LOG_LEVEL,helics.HelicsLogLevels.HELICS_LOG_LEVEL_WARNING);
catch ec
    success=false;
    helics.helicsBrokerDestroy(fedStruct.broker);
    helics.helicsFederateInfoFree(fedInfo);
    return
end
try
fedStruct.vFed=helics.helicsCreateValueFederate('fed1',fedInfo);
if (~helics.helicsFederateIsValid(fedStruct.vFed))
    success=false;
end
catch ec
    success=false;
end
helics.helicsFederateInfoFree(fedInfo);
end

function success=closeStruct(fedStruct)
import helics.*
success=true;
helics.helicsFederateFinalize(fedStruct.vFed);
helics.helicsBrokerWaitForDisconnect(fedStruct.broker,2000);

helics.helicsFederateFree(fedStruct.vFed);
helics.helicsBrokerFree(fedStruct.broker);
helics.helicsCloseLibrary();

end

function forceCloseStruct(fedStruct)
import helics.*
helics.helicsFederateFinalize(fedStruct.vFed);

cnt=0;
while (helics.helicsBrokerIsConnected(fedStruct.broker))
    pause(1);
    cnt=cnt+1;
    if (cnt>5)
        helics.helicsBrokerDisconnect(fedStruct.broker);
        break;
    end
end

helics.helicsFederateFree(fedStruct.vFed);
helics.helicsBrokerFree(fedStruct.broker);
helics.helicsCloseLibrary();

end

function testBasic(testCase)
import matlab.unittest.constraints.IsGreaterThan
import helics.*
ver=helics.helicsGetVersion();
verifyThat(testCase,length(ver),IsGreaterThan(10),'version length is not valid');
end

function testBrokerStatus(testCase)
import helics.*
import matlab.unittest.constraints.IsTrue
[feds,success]=generateFed();
testCase.verifyThat(success,IsTrue);
if (~success)
    forceCloseStruct(feds);
    return
end
try
    testCase.verifyThat(helics.helicsBrokerIsRoot(feds.broker) ~= helics.HELICS_FALSE(),IsTrue,'broker should be a root broker');
    testCase.verifyThat(helics.helicsBrokerIsOpenToNewFederates(feds.broker) ~= helics.HELICS_FALSE(),IsTrue,'root broker should be open to new federates');
catch e
    forceCloseStruct(feds);
    rethrow(e);
end
success=closeStruct(feds);
testCase.verifyThat(success,IsTrue);
end

function testEnterExecution(testCase)
import matlab.unittest.constraints.IsTrue;
[feds,success]=generateFed();
try
testCase.verifyThat(success,IsTrue);
helics.helicsFederateEnterExecutingMode(feds.vFed);
state=helics.helicsFederateGetState(feds.vFed);
testCase.verifyEqual(state,helics.HelicsFederateState.HELICS_STATE_EXECUTION);
success=closeStruct(feds);
testCase.verifyThat(success,IsTrue);
catch e
    testCase.verifyThat(false,IsTrue);
    disp(e.message)
    disp(e.stack(1))
    forceCloseStruct(feds);
end
end

function testPublicationRegistration(testCase)
import matlab.unittest.constraints.IsTrue;
import helics.*
[feds,success]=generateFed();
testCase.verifyThat(success,IsTrue);
try
pubid1 = helics.helicsFederateRegisterTypePublication(feds.vFed, 'pub1', 'string', '');
pubid2 = helics.helicsFederateRegisterGlobalTypePublication(feds.vFed, 'pub2', 'int', '');
pubid3 = helics.helicsFederateRegisterTypePublication(feds.vFed, 'pub3', 'double', 'V');
helics.helicsFederateEnterExecutingMode(feds.vFed);

publication_key = helics.helicsPublicationGetName(pubid1);
testCase.verifyEqual(publication_key,'fed1/pub1');
publication_key = helics.helicsPublicationGetName(pubid2);
testCase.verifyEqual(publication_key,'pub2');
publication_key = helics.helicsPublicationGetName(pubid3);
testCase.verifyEqual(publication_key,'fed1/pub3');
publication_type = helics.helicsPublicationGetType(pubid3);
testCase.verifyEqual(publication_type,'double');
publication_units = helics.helicsPublicationGetUnits(pubid3);
testCase.verifyEqual(publication_units,'V');
success=closeStruct(feds);
testCase.verifyThat(success,IsTrue);
catch e
    testCase.verifyThat(false,IsTrue);
   disp(e.message)
    disp(e.stack(1))
    forceCloseStruct(feds);
end
end

function testNamedPoint(testCase)
import matlab.unittest.constraints.IsTrue;
import helics.*
[feds,success]=generateFed();
testCase.verifyThat(success,IsTrue);

defaultValue = 'start';
    defVal = 5.3;
    testValue1 = 'inside of the functional relationship of helics';
    testVal1 = 45.7823;
    testValue2 = 'I am a string';
    testVal2 = 0.0;
try
    pubid = helics.helicsFederateRegisterGlobalPublication(feds.vFed, 'pub1', helics.HelicsDataTypes.HELICS_DATA_TYPE_NAMED_POINT, '');
    subid = helics.helicsFederateRegisterSubscription(feds.vFed, 'pub1', '');

    helics.helicsInputSetDefaultNamedPoint(subid, defaultValue, defVal);

    helics.helicsFederateEnterExecutingMode(feds.vFed);

    % publish string1 at time=0.0;
    helics.helicsPublicationPublishNamedPoint(pubid, testValue1, testVal1);

    % double val;
    [value,val] = helics.helicsInputGetNamedPoint(subid);
    testCase.verifyEqual(value,defaultValue);
    testCase.verifyEqual(val,defVal);

    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 1.0);
    testCase.verifyEqual(grantedtime,0.01);

    % get the value
    [value,val] = helics.helicsInputGetNamedPoint(subid);
    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue1);
    testCase.verifyEqual(val,testVal1);

    % publish a second string
    helics.helicsPublicationPublishNamedPoint(pubid, testValue2, testVal2);

    % make sure the value is still what we expect
    [value, val] = helics.helicsInputGetNamedPoint(subid);
    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue1);
    testCase.verifyEqual(val,testVal1);

    % advance time
    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 2.0);
    testCase.verifyEqual(grantedtime,0.02);

    % make sure the value was updated
    [value, val] = helics.helicsInputGetNamedPoint(subid);
    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue2);
    testCase.verifyEqual(val,testVal2);
    success=closeStruct(feds);
    testCase.verifyThat(success,IsTrue);
catch e
    testCase.verifyThat(false,IsTrue);
    disp(e.message)
    disp(e.stack(1))
    forceCloseStruct(feds);
end
end

function testBool(testCase)
import matlab.unittest.constraints.IsTrue;
import helics.*
[feds,success]=generateFed();
testCase.verifyThat(success,IsTrue);

try
defaultValue = helics.HELICS_TRUE;
    testValue1 = helics.HELICS_TRUE;

    pubid = helics.helicsFederateRegisterGlobalPublication(feds.vFed, 'pub1', helics.HelicsDataTypes.HELICS_DATA_TYPE_BOOLEAN, '');
    subid = helics.helicsFederateRegisterSubscription(feds.vFed, 'pub1', '');

    helics.helicsInputSetDefaultBoolean(subid, defaultValue);


    helics.helicsFederateEnterExecutingMode(feds.vFed);

    % publish string1 at time=0.0;
     helics.helicsPublicationPublishBoolean(pubid, testValue1);


    % double val;
    value = helics.helicsInputGetBoolean(subid);

    testCase.verifyEqual(value,defaultValue);

    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 1.0);

    testCase.verifyEqual(grantedtime,0.01);

    % get the value
    value = helics.helicsInputGetBoolean(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue1);

    % publish a second boolean
    helics.helicsPublicationPublish(pubid, false);


    % make sure the value is still what we expect
    value = helics.helicsInputGetBoolean(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue1);

    % advance time
    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 2.0);

    testCase.verifyEqual(grantedtime,0.02);

    % make sure the value was updated
    value = helics.helicsInputGetBoolean(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,helics.HELICS_FALSE);
    success=closeStruct(feds);
    testCase.verifyThat(success,IsTrue);
catch e
    testCase.verifyThat(false,IsTrue);
    disp(e.message)
    disp(e.stack(1))
    forceCloseStruct(feds);
end
end

function testPublisherRegistration(testCase)
import matlab.unittest.constraints.IsTrue;
import helics.*
[feds,success]=generateFed();
testCase.verifyThat(success,IsTrue);

try
    pubid1 = helics.helicsFederateRegisterPublication(feds.vFed, 'pub1', helics.HelicsDataTypes.HELICS_DATA_TYPE_STRING, '');
    pubid2 = helics.helicsFederateRegisterGlobalPublication(feds.vFed, 'pub2', helics.HelicsDataTypes.HELICS_DATA_TYPE_INT, '');
    pubid3 = helics.helicsFederateRegisterPublication(feds.vFed, 'pub3', helics.HelicsDataTypes.HELICS_DATA_TYPE_DOUBLE, 'V');

    publication_key = helics.helicsPublicationGetName(pubid1);

testCase.verifyEqual(publication_key,'fed1/pub1');
publication_type = helics.helicsPublicationGetType(pubid1);

testCase.verifyEqual(publication_type,'string');
publication_key = helics.helicsPublicationGetName(pubid2);

testCase.verifyEqual(publication_key,'pub2');
publication_type = helics.helicsPublicationGetType(pubid2);

testCase.verifyEqual(publication_type,'int64');
publication_key = helics.helicsPublicationGetName(pubid3);

testCase.verifyEqual(publication_key,'fed1/pub3');
publication_type = helics.helicsPublicationGetType(pubid3);

testCase.verifyEqual(publication_type,'double');
publication_units = helics.helicsPublicationGetUnits(pubid3);

testCase.verifyEqual(publication_units,'V');

    helics.helicsFederateEnterExecutingMode(feds.vFed);


   %% add state and some type checks
    success=closeStruct(feds);
    testCase.verifyThat(success,IsTrue);
catch e
    testCase.verifyThat(false,IsTrue);
    disp(e.message)
    disp(e.stack(1))
    forceCloseStruct(feds);
end
end

function testDouble(testCase)
import matlab.unittest.constraints.IsTrue;
import helics.*
[feds,success]=generateFed();
testCase.verifyThat(success,IsTrue);

try
defaultValue = 1.0;
    testValue1 = 2.7586;
    testValue2 = 1e27;

    pubid = helics.helicsFederateRegisterGlobalPublication(feds.vFed, 'pub1', helics.HelicsDataTypes.HELICS_DATA_TYPE_DOUBLE, '');
    subid = helics.helicsFederateRegisterSubscription(feds.vFed, 'pub1', '');

    helics.helicsInputSetDefault(subid, defaultValue);


    helics.helicsFederateEnterExecutingMode(feds.vFed);


    % publish string1 at time=0.0;
    helics.helicsPublicationPublishDouble(pubid, testValue1);


    % double val;
    value = helics.helicsInputGetDouble(subid);

    testCase.verifyEqual(value,defaultValue);

    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 1.0);

    testCase.verifyEqual(grantedtime,0.01);

    % get the value
    value = helics.helicsInputGetDouble(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue1);

    % publish a second double
    helics.helicsPublicationPublish(pubid, testValue2);


    % make sure the value is still what we expect
    value = helics.helicsInputGetDouble(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue1);

    % advance time
    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 2.0);

    testCase.verifyEqual(grantedtime,0.02);

    % make sure the value was updated
    value = helics.helicsInputGetDouble(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue2);
    success=closeStruct(feds);
    testCase.verifyThat(success,IsTrue);
catch e
    testCase.verifyThat(false,IsTrue);
    disp(e.message)
    disp(e.stack(1))
    forceCloseStruct(feds);
end
end


function testComplex(testCase)
import matlab.unittest.constraints.IsTrue;
import helics.*
[feds,success]=generateFed();
testCase.verifyThat(success,IsTrue);

try
defaultValue = 1.0-1.0j;
    testValue1 = 2.7586+ 342.25626j;
    testValue2 = 1e27-0.3e-2j;

    pubid = helics.helicsFederateRegisterGlobalPublication(feds.vFed, 'pub1', helics.HelicsDataTypes.HELICS_DATA_TYPE_COMPLEX, '');
    subid = helics.helicsFederateRegisterSubscription(feds.vFed, 'pub1', '');

    helics.helicsInputSetDefault(subid, defaultValue);


    helics.helicsFederateEnterExecutingMode(feds.vFed);


    % publish string1 at time=0.0;
    helics.helicsPublicationPublishComplex(pubid, testValue1);


    % double val;
    value = helics.helicsInputGetComplex(subid);

    testCase.verifyEqual(value,defaultValue);

    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 1.0);

    testCase.verifyEqual(grantedtime,0.01);

    % get the value
    value = helics.helicsInputGetComplex(subid);

    % make sure the value is what we expect
    testCase.verifyEqual(value,testValue1);

    % publish a second complex value
    helics.helicsPublicationPublish(pubid, testValue2);


    % make sure the value is still what we expect
    value = helics.helicsInputGetComplex(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue1);
    % advance time
    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 2.0);

    testCase.verifyEqual(grantedtime,0.02);

    % make sure the value was updated
    value = helics.helicsInputGetComplex(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue2);
    success=closeStruct(feds);
    testCase.verifyThat(success,IsTrue);
catch e
    testCase.verifyThat(false,IsTrue);
    disp(e.message)
    disp(e.stack(1))
    forceCloseStruct(feds);
end
end


function testInteger(testCase)
import matlab.unittest.constraints.IsTrue;
import helics.*
[feds,success]=generateFed();
testCase.verifyThat(success,IsTrue);

try
defaultValue = int64(45626678);
    testValue1 = int64(-27);
    testValue2 = int64(0);

    pubid = helics.helicsFederateRegisterGlobalPublication(feds.vFed, 'pub1', helics.HelicsDataTypes.HELICS_DATA_TYPE_INT, '');
    subid = helics.helicsFederateRegisterSubscription(feds.vFed, 'pub1', '');

    helics.helicsInputSetDefault(subid, defaultValue);


    helics.helicsFederateEnterExecutingMode(feds.vFed);


    % publish string1 at time=0.0;
    helics.helicsPublicationPublishInteger(pubid, testValue1);


    % double val;
    value = helics.helicsInputGetInteger(subid);

    testCase.verifyEqual(value,defaultValue);

    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 1.0);

    testCase.verifyEqual(grantedtime,0.01);

    % get the value
    value = helics.helicsInputGetInteger(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue1);

    % publish a second string
    helics.helicsPublicationPublish(pubid, testValue2);

    % make sure the value is still what we expect
    value = helics.helicsInputGetInteger(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue1);

    % advance time
    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 2.0);

    testCase.verifyEqual(grantedtime,0.02);

    % make sure the value was updated
    value = helics.helicsInputGetInteger(subid);

    % make sure the value is what we expect
    testCase.verifyEqual(value,testValue2);
    success=closeStruct(feds);
    testCase.verifyThat(success,IsTrue);
catch e
    testCase.verifyThat(false,IsTrue);
    disp(e.message)
    disp(e.stack(1))
    forceCloseStruct(feds);
end
end


function testString(testCase)
import matlab.unittest.constraints.IsTrue;
import helics.*
[feds,success]=generateFed();
testCase.verifyThat(success,IsTrue);

try
defaultValue = 'string1';
    testValue1 = 'this is a longer test string to bypass sso';
    testValue2 = '';

    pubid = helics.helicsFederateRegisterGlobalPublication(feds.vFed, 'pub1', helics.HelicsDataTypes.HELICS_DATA_TYPE_STRING, '');
    subid = helics.helicsFederateRegisterSubscription(feds.vFed, 'pub1', '');

    helics.helicsInputSetDefault(subid, defaultValue);

    helics.helicsFederateEnterExecutingMode(feds.vFed);

    % publish string1 at time=0.0;
    helics.helicsPublicationPublish(pubid, testValue1);


    % double val;
    value = helics.helicsInputGetString(subid);

    testCase.verifyEqual(value,defaultValue);

    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 1.0);

    testCase.verifyEqual(grantedtime,0.01);

    % get the value
    value = helics.helicsInputGetString(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue1);

    % publish a second string
    helics.helicsPublicationPublishString(pubid, testValue2);


    % make sure the value is still what we expect
    value = helics.helicsInputGetString(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue1);

    % advance time
    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 2.0);

    testCase.verifyEqual(grantedtime,0.02);

    % make sure the value was updated
    value = helics.helicsInputGetString(subid);

    % make sure the value is what we expect
    testCase.verifyEqual(isempty(value),isempty(testValue2));
    success=closeStruct(feds);
    testCase.verifyThat(success,IsTrue);
catch e
    testCase.verifyThat(false,IsTrue);
    disp(e.message)
    disp(e.stack(1))
    forceCloseStruct(feds);
end
end


function testVector(testCase)
import matlab.unittest.constraints.IsTrue;
import helics.*
[feds,success]=generateFed();
testCase.verifyThat(success,IsTrue);

try
defaultValue = [34.5;22.1;-10.4];
    testValue1 = ones(22,1);
    testValue2 = [99.1;-99;2;0.0;-1e35;4.56e-7];

    pubid = helics.helicsFederateRegisterGlobalPublication(feds.vFed, 'pub1', helics.HelicsDataTypes.HELICS_DATA_TYPE_VECTOR, '');
    subid = helics.helicsFederateRegisterSubscription(feds.vFed, 'pub1', '');

    helics.helicsInputSetDefault(subid, defaultValue);


    helics.helicsFederateEnterExecutingMode(feds.vFed);


    % publish string1 at time=0.0;
    helics.helicsPublicationPublishVector(pubid, testValue1);


    % double val;
    value = helics.helicsInputGetVector(subid);

    testCase.verifyEqual(value,defaultValue);

    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 1.0);

    testCase.verifyEqual(grantedtime,0.01);

    % get the value
    value = helics.helicsInputGetVector(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue1);

    % publish a second string
    helics.helicsPublicationPublish(pubid, testValue2);


    % make sure the value is still what we expect
    value = helics.helicsInputGetVector(subid);

    % make sure the string is what we expect
    testCase.verifyEqual(value,testValue1);

    % advance time
    grantedtime = helics.helicsFederateRequestTime(feds.vFed, 2.0);

    testCase.verifyEqual(grantedtime,0.02);

    % make sure the value was updated
    value = helics.helicsInputGetVector(subid);

    % make sure the value is what we expect
    testCase.verifyEqual(isempty(value),isempty(testValue2));
    success=closeStruct(feds);
    testCase.verifyThat(success,IsTrue);
catch e
    testCase.verifyThat(false,IsTrue);
    disp(e.message)
    disp(e.stack(1))
    forceCloseStruct(feds);
end
end
