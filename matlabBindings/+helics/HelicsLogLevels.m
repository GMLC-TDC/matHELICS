% log level definitions
%
% Attributes:
%	HELICS_LOG_LEVEL_DUMPLOG: value:-10	log level for dumping log messages
%	HELICS_LOG_LEVEL_NO_PRINT: value:-4	don't print anything except a few catastrophic errors
%	HELICS_LOG_LEVEL_ERROR: value:0	only print error level indicators
%	HELICS_LOG_LEVEL_PROFILING: value:2	profiling log level
%	HELICS_LOG_LEVEL_WARNING: value:3	only print warnings and errors
%	HELICS_LOG_LEVEL_SUMMARY: value:6	warning errors and summary level information
%	HELICS_LOG_LEVEL_CONNECTIONS: value:9	summary+ notices about federate and broker connections +messages about network connections
%	HELICS_LOG_LEVEL_INTERFACES: value:12	connections+ interface definitions
%	HELICS_LOG_LEVEL_TIMING: value:15	interfaces + timing message
%	HELICS_LOG_LEVEL_DATA: value:18	timing+ data transfer notices
%	HELICS_LOG_LEVEL_DEBUG: value:21	data+ additional debug message
%	HELICS_LOG_LEVEL_TRACE: value:24	all internal messages
classdef HelicsLogLevels
	properties (Constant)
		HELICS_LOG_LEVEL_DUMPLOG = int32(-10);
		HELICS_LOG_LEVEL_NO_PRINT = int32(-4);
		HELICS_LOG_LEVEL_ERROR = int32(0);
		HELICS_LOG_LEVEL_PROFILING = int32(2);
		HELICS_LOG_LEVEL_WARNING = int32(3);
		HELICS_LOG_LEVEL_SUMMARY = int32(6);
		HELICS_LOG_LEVEL_CONNECTIONS = int32(9);
		HELICS_LOG_LEVEL_INTERFACES = int32(12);
		HELICS_LOG_LEVEL_TIMING = int32(15);
		HELICS_LOG_LEVEL_DATA = int32(18);
		HELICS_LOG_LEVEL_DEBUG = int32(21);
		HELICS_LOG_LEVEL_TRACE = int32(24);
	end
end