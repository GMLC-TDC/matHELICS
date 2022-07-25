%% a single federate example for HELICS in matlab

helicsStartup
import helics.*
% create a broker so we don't have to do this separately
brk=helicsCreateBroker('B1','-f1');
% create a combination federate so we add both endpoint and
% publications/inputs
fed1=helicsCreateCombinationFederate('fed1','');
helicsFederateSetOption(fed1,HelicsFederateFlags.HELICS_FLAG_UNINTERRUPTIBLE)
% register Endpoint1/2
e1=helicsFederateRegisterGlobalEndpoint('Endpoint1','');
e2=helicsFederateRegisterGlobalEndpoint('Endpoint2','');

% register a publication and subscription

pub1=helicsFederateRegisterGlobalPublication('pub1',HelicsDataTypes.HELICS_DATA_TYPE_DOUBLE,'V');

sub1=helicsFederateRegisterSubscription('pub1','kV');

% enter initializing mode

helicsFederateEnterInitializingMode();

helicsPublicationPublishDouble(11200);

% enter executing mode
helicsFederateEnterExecutingMode()

val=helicsInputGetValue(sub1);
% we can publish in any form (though it will get converted to a double)
helicsPublicationPublishString('11400');
% the value will be in kV so should read 11.2 instead of 11200 which we
% published due to the unit conversion
fprintf('received value %f at time %f\n',val,helicsFederateGetCurrentTime(fed1));

helicsEndpointSendTo(e1,'Test Message 1','Endpoint2');

% we set the federate to uninterruptible so it should grant a time of 2.0
t2=helicsFederateRequestTime(2.0);

fprintf('Granted time of %f\n',t2);

val=helicsInputGetValue(sub1);
% we can publish in any form (though it will get converted to a double)
helicsPublicationPublishString('11400');

fprintf('received value %f at time %f\n',val,t2);

if (helicsEndpointHasMessage(e2))
    m=helicsEndpointGetMessage(e2);
    fprintf('recevied message from %s at time %f: %s\n',helicsMesssageGetSource(m),t2,helicsMessageGetString(m));
end

%disconnect the federate
helicsFederateDisconnect(fed1);
%now the broker should terminate the cosimulation
helicsBrokerWaitForDisconnect(brk);
%close up the library and clean up any memory use that hasn't been cleaned
%already.  
helicsCloseLibrary();
