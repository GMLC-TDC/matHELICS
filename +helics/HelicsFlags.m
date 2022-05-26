%{
enumeration of general flags that can be used in federates/cores/brokers

Attributes:
	HELICS_FLAG_SLOW_RESPONDING: value:29	flag specifying that a federate, core, or broker may be slow to respond to pings If the federate goes offline there is no good way to detect it so use with caution
	HELICS_FLAG_DEBUGGING: value:31	flag specifying the federate/core/broker is operating in a user debug mode so deadlock timers and timeout are disabled this flag is a combination of slow_responding and disabling of some timeouts
	HELICS_FLAG_TERMINATE_ON_ERROR: value:72	specify that a federate error should terminate the federation
	HELICS_FLAG_FORCE_LOGGING_FLUSH: value:88	specify that the log files should be flushed on every log message
	HELICS_FLAG_DUMPLOG: value:89	specify that a full log should be dumped into a file
	HELICS_FLAG_PROFILING: value:93	specify that helics should capture profiling data
	HELICS_FLAG_PROFILING_MARKER: value:95	flag trigger for generating a profiling marker
%}
HELICS_FLAG_SLOW_RESPONDING = 29;
HELICS_FLAG_DEBUGGING = 31;
HELICS_FLAG_TERMINATE_ON_ERROR = 72;
HELICS_FLAG_FORCE_LOGGING_FLUSH = 88;
HELICS_FLAG_DUMPLOG = 89;
HELICS_FLAG_PROFILING = 93;
HELICS_FLAG_PROFILING_MARKER = 95;