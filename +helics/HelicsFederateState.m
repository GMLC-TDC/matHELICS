%{
enumeration of possible federate states

Attributes:
	HELICS_STATE_STARTUP: value:0	when created the federate is in startup state
	HELICS_STATE_INITIALIZATION: value:1	entered after the enterInitializingMode call has returned
	HELICS_STATE_EXECUTION: value:2	entered after the enterExectuationState call has returned
	HELICS_STATE_FINALIZE: value:3	the federate has finished executing normally final values may be retrieved
	HELICS_STATE_ERROR: value:4	error state no core communication is possible but values can be retrieved
	HELICS_STATE_PENDING_INIT: value:5	indicator that the federate is pending entry to initialization state
	HELICS_STATE_PENDING_EXEC: value:6	state pending EnterExecution State
	HELICS_STATE_PENDING_TIME: value:7	state that the federate is pending a timeRequest
	HELICS_STATE_PENDING_ITERATIVE_TIME: value:8	state that the federate is pending an iterative time request
	HELICS_STATE_PENDING_FINALIZE: value:9	state that the federate is pending a finalize request
	HELICS_STATE_FINISHED: value:10	state that the federate is finished simulating but still connected
%}
classdef HelicsFederateState
	properties (Constant)
		HELICS_STATE_STARTUP = int32(0);
		HELICS_STATE_INITIALIZATION = int32(1);
		HELICS_STATE_EXECUTION = int32(2);
		HELICS_STATE_FINALIZE = int32(3);
		HELICS_STATE_ERROR = int32(4);
		HELICS_STATE_PENDING_INIT = int32(5);
		HELICS_STATE_PENDING_EXEC = int32(6);
		HELICS_STATE_PENDING_TIME = int32(7);
		HELICS_STATE_PENDING_ITERATIVE_TIME = int32(8);
		HELICS_STATE_PENDING_FINALIZE = int32(9);
		HELICS_STATE_FINISHED = int32(10);
	end
end