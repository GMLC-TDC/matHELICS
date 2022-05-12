classdef HelicsLogLevels < uint64
	%{
	log level definitions

	Attributes:
		HELICS_LOG_LEVEL_DUMPLOG: value:-10	log level for dumping log messages
		HELICS_LOG_LEVEL_NO_PRINT: value:-4	don't print anything except a few catastrophic errors
		HELICS_LOG_LEVEL_ERROR: value:0	only print error level indicators
		HELICS_LOG_LEVEL_PROFILING: value:2	profiling log level
		HELICS_LOG_LEVEL_WARNING: value:3	only print warnings and errors
		HELICS_LOG_LEVEL_SUMMARY: value:6	warning errors and summary level information
		HELICS_LOG_LEVEL_CONNECTIONS: value:9	summary+ notices about federate and broker connections +messages about network connections
		HELICS_LOG_LEVEL_INTERFACES: value:12	connections+ interface definitions
		HELICS_LOG_LEVEL_TIMING: value:15	interfaces + timing message
		HELICS_LOG_LEVEL_DATA: value:18	timing+ data transfer notices
		HELICS_LOG_LEVEL_DEBUG: value:21	data+ additional debug message
		HELICS_LOG_LEVEL_TRACE: value:24	all internal messages
	%}
	enumeration
		HELICS_LOG_LEVEL_DUMPLOG (-10)
		HELICS_LOG_LEVEL_NO_PRINT (-4)
		HELICS_LOG_LEVEL_ERROR (0)
		HELICS_LOG_LEVEL_PROFILING (2)
		HELICS_LOG_LEVEL_WARNING (3)
		HELICS_LOG_LEVEL_SUMMARY (6)
		HELICS_LOG_LEVEL_CONNECTIONS (9)
		HELICS_LOG_LEVEL_INTERFACES (12)
		HELICS_LOG_LEVEL_TIMING (15)
		HELICS_LOG_LEVEL_DATA (18)
		HELICS_LOG_LEVEL_DEBUG (21)
		HELICS_LOG_LEVEL_TRACE (24)
	end
end