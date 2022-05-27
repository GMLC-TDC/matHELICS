%{
enumeration of possible federate flags

Attributes:
	HELICS_FLAG_OBSERVER: value:0	flag indicating that a federate is observe only
	HELICS_FLAG_UNINTERRUPTIBLE: value:1	flag indicating that a federate can only return requested times
	HELICS_FLAG_INTERRUPTIBLE: value:2	flag indicating that a federate can be interrupted
	HELICS_FLAG_SOURCE_ONLY: value:4	flag indicating that a federate/interface is a signal generator only
	HELICS_FLAG_ONLY_TRANSMIT_ON_CHANGE: value:6	flag indicating a federate/interface should only transmit values if they have changed(binary equivalence)
	HELICS_FLAG_ONLY_UPDATE_ON_CHANGE: value:8	flag indicating a federate/interface should only trigger an update if a value has changed (binary equivalence)
	HELICS_FLAG_WAIT_FOR_CURRENT_TIME_UPDATE: value:10	flag indicating a federate should only grant time if all other federates have already passed the requested time
	HELICS_FLAG_RESTRICTIVE_TIME_POLICY: value:11	flag indicating a federate should operate on a restrictive time policy, which disallows some 2nd order time evaluation and can be useful for certain types of dependency cycles and update patterns, but generally shouldn't be used as it can lead to some very slow update conditions
	HELICS_FLAG_ROLLBACK: value:12	flag indicating that a federate has rollback capability
	HELICS_FLAG_FORWARD_COMPUTE: value:14	flag indicating that a federate performs forward computation and does internal rollback
	HELICS_FLAG_REALTIME: value:16	flag indicating that a federate needs to run in real time
	HELICS_FLAG_SINGLE_THREAD_FEDERATE: value:27	flag indicating that the federate will only interact on a single thread
	HELICS_FLAG_IGNORE_TIME_MISMATCH_WARNINGS: value:67	used to not display warnings on mismatched requested times
	HELICS_FLAG_STRICT_CONFIG_CHECKING: value:75	specify that checking on configuration files should be strict and throw and error on any invalid values
	HELICS_FLAG_USE_JSON_SERIALIZATION: value:79	specify that the federate should use json serialization for all data types
	HELICS_FLAG_EVENT_TRIGGERED: value:81	specify that the federate is event triggered-meaning (all/most) events are triggered by incoming events
	HELICS_FLAG_LOCAL_PROFILING_CAPTURE: value:96	specify that that federate should capture the profiling data to the local federate logging system
%}
classdef HelicsFederateFlags
	properties (Constant)
		HELICS_FLAG_OBSERVER = int32(0);
		HELICS_FLAG_UNINTERRUPTIBLE = int32(1);
		HELICS_FLAG_INTERRUPTIBLE = int32(2);
		HELICS_FLAG_SOURCE_ONLY = int32(4);
		HELICS_FLAG_ONLY_TRANSMIT_ON_CHANGE = int32(6);
		HELICS_FLAG_ONLY_UPDATE_ON_CHANGE = int32(8);
		HELICS_FLAG_WAIT_FOR_CURRENT_TIME_UPDATE = int32(10);
		HELICS_FLAG_RESTRICTIVE_TIME_POLICY = int32(11);
		HELICS_FLAG_ROLLBACK = int32(12);
		HELICS_FLAG_FORWARD_COMPUTE = int32(14);
		HELICS_FLAG_REALTIME = int32(16);
		HELICS_FLAG_SINGLE_THREAD_FEDERATE = int32(27);
		HELICS_FLAG_IGNORE_TIME_MISMATCH_WARNINGS = int32(67);
		HELICS_FLAG_STRICT_CONFIG_CHECKING = int32(75);
		HELICS_FLAG_USE_JSON_SERIALIZATION = int32(79);
		HELICS_FLAG_EVENT_TRIGGERED = int32(81);
		HELICS_FLAG_LOCAL_PROFILING_CAPTURE = int32(96);
	end
end