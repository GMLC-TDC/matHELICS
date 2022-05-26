%{
enumeration of properties that apply to federates and sometimes cores

Attributes:
	HELICS_PROPERTY_TIME_DELTA: value:137	the property controlling the minimum time delta for a federate
	HELICS_PROPERTY_TIME_PERIOD: value:140	the property controlling the period for a federate
	HELICS_PROPERTY_TIME_OFFSET: value:141	the property controlling time offset for the period of federate
	HELICS_PROPERTY_TIME_RT_LAG: value:143	the property controlling real time lag for a federate the max time a federate can lag real time
	HELICS_PROPERTY_TIME_RT_LEAD: value:144	the property controlling real time lead for a federate the max time a federate can be ahead of real time
	HELICS_PROPERTY_TIME_RT_TOLERANCE: value:145	the property controlling real time tolerance for a federate sets both rt_lag and rt_lead
	HELICS_PROPERTY_TIME_INPUT_DELAY: value:148	the property controlling input delay for a federate
	HELICS_PROPERTY_TIME_OUTPUT_DELAY: value:150	the property controlling output delay for a federate
	HELICS_PROPERTY_TIME_GRANT_TIMEOUT: value:161	the property specifying a timeout to trigger actions if the time for granting exceeds a certain threshold
	HELICS_PROPERTY_INT_MAX_ITERATIONS: value:259	integer property controlling the maximum number of iterations in a federate
	HELICS_PROPERTY_INT_LOG_LEVEL: value:271	integer property controlling the log level in a federate see HelicsLogLevels
	HELICS_PROPERTY_INT_FILE_LOG_LEVEL: value:272	integer property controlling the log level for file logging in a federate see HelicsLogLevels
	HELICS_PROPERTY_INT_CONSOLE_LOG_LEVEL: value:274	integer property controlling the log level for console logging in a federate see HelicsLogLevels
	HELICS_PROPERTY_INT_LOG_BUFFER: value:276	integer property controlling the size of the log buffer
%}
classdef HelicsProperties
	properties (Constant)
		HELICS_PROPERTY_TIME_DELTA = int32(137);
		HELICS_PROPERTY_TIME_PERIOD = int32(140);
		HELICS_PROPERTY_TIME_OFFSET = int32(141);
		HELICS_PROPERTY_TIME_RT_LAG = int32(143);
		HELICS_PROPERTY_TIME_RT_LEAD = int32(144);
		HELICS_PROPERTY_TIME_RT_TOLERANCE = int32(145);
		HELICS_PROPERTY_TIME_INPUT_DELAY = int32(148);
		HELICS_PROPERTY_TIME_OUTPUT_DELAY = int32(150);
		HELICS_PROPERTY_TIME_GRANT_TIMEOUT = int32(161);
		HELICS_PROPERTY_INT_MAX_ITERATIONS = int32(259);
		HELICS_PROPERTY_INT_LOG_LEVEL = int32(271);
		HELICS_PROPERTY_INT_FILE_LOG_LEVEL = int32(272);
		HELICS_PROPERTY_INT_CONSOLE_LOG_LEVEL = int32(274);
		HELICS_PROPERTY_INT_LOG_BUFFER = int32(276);
	end
end