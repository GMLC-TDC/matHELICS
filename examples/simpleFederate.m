%% a single federate example for HELICS in matlab

%helicsStartup
%HELICS_PATH = 'C:\Users\mukh915\matHELICS\helics'; % matHELICS path
HELICS_PATH = '/home/helics-user/Softwares_user/matHELICS/'; % matHELICS path
addpath(HELICS_PATH)
% Checking if Octave or Matlab
isOctave = exist('OCTAVE_VERSION', 'builtin') ~= 0;
if isOctave
  % Using custom Import as the import function is not yet implemented in Octave
  addpath(fullfile(HELICS_PATH, '+helics'))
else
  import helics.*
end

% create a broker so we don't have to do this separately
brk=helicsCreateBroker('','B1','-f1');
% create a combination federate so we add both endpoint and
% publications/inputs
fi=helicsCreateFederateInfo();
fed1=helicsCreateCombinationFederate('fed1',fi);
helicsFederateSetFlagOption(fed1, HelicsFederateFlags.HELICS_FLAG_UNINTERRUPTIBLE, HELICS_TRUE)
% register Endpoint1/2
e1=helicsFederateRegisterGlobalEndpoint(fed1,'Endpoint1','');
e2=helicsFederateRegisterGlobalEndpoint(fed1,'Endpoint2','');

% register a publication and subscription

pub1=helicsFederateRegisterGlobalPublication(fed1,'pub1',HelicsDataTypes.HELICS_DATA_TYPE_DOUBLE,'V');

sub1=helicsFederateRegisterSubscription(fed1,'pub1','kV');

% enter initializing mode

helicsFederateEnterInitializingMode(fed1);

helicsPublicationPublishDouble(pub1,11200);

% enter executing mode
helicsFederateEnterExecutingMode(fed1)

val=helicsInputGetDouble(sub1);
% we can publish in any form (though it will get converted to a double)
helicsPublicationPublishString(pub1,'11400');
% the value will be in kV so should read 11.2 instead of 11200 which we
% published due to the unit conversion
fprintf('received value %f at time %f\n',val,helicsFederateGetCurrentTime(fed1));

helicsEndpointSendBytesTo(e1,'Test Message 1','Endpoint2');

% we set the federate to uninterruptible so it should grant a time of 2.0
t2=helicsFederateRequestTime(fed1,2.0);

fprintf('Granted time of %f\n',t2);

val=helicsInputGetDouble(sub1);
% we can publish in any form (though it will get converted to a double)
helicsPublicationPublishString(pub1,'11400');

fprintf('received value %f at time %f\n',val,t2);

if (helicsEndpointHasMessage(e2))
    m=helicsEndpointGetMessage(e2);
    fprintf('recevied message from %s at time %f: %s\n',helicsMessageGetSource(m),t2,helicsMessageGetString(m));
end

%disconnect the federate
helicsFederateDisconnect(fed1);
%now the broker should terminate the cosimulation
helicsBrokerWaitForDisconnect(brk,0);
%close up the library and clean up any memory use that hasn't been cleaned
%already.
helicsCloseLibrary();
