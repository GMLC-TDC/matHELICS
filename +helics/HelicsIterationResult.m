classdef HelicsIterationResult < uint64
	%{
	enumeration of possible return values from an iterative time request

	Attributes:
		HELICS_ITERATION_RESULT_NEXT_STEP: value:0	the iterations have progressed to the next time
		HELICS_ITERATION_RESULT_ERROR: value:1	there was an error
		HELICS_ITERATION_RESULT_HALTED: value:2	the federation has halted
		HELICS_ITERATION_RESULT_ITERATING: value:3	the federate is iterating at current time
	%}
	enumeration
		HELICS_ITERATION_RESULT_NEXT_STEP (0)
		HELICS_ITERATION_RESULT_ERROR (1)
		HELICS_ITERATION_RESULT_HALTED (2)
		HELICS_ITERATION_RESULT_ITERATING (3)
	end
end