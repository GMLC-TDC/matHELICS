%{
enumeration of possible return values from an iterative time request

Attributes:
	HELICS_ITERATION_RESULT_NEXT_STEP: value:0	the iterations have progressed to the next time
	HELICS_ITERATION_RESULT_ERROR: value:1	there was an error
	HELICS_ITERATION_RESULT_HALTED: value:2	the federation has halted
	HELICS_ITERATION_RESULT_ITERATING: value:3	the federate is iterating at current time
%}
classdef HelicsIterationResult
	properties (Constant)
		HELICS_ITERATION_RESULT_NEXT_STEP = int32(0);
		HELICS_ITERATION_RESULT_ERROR = int32(1);
		HELICS_ITERATION_RESULT_HALTED = int32(2);
		HELICS_ITERATION_RESULT_ITERATING = int32(3);
	end
end