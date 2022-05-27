%{
enumeration of the different iteration results

Attributes:
	HELICS_ITERATION_REQUEST_NO_ITERATION: value:0	no iteration is requested
	HELICS_ITERATION_REQUEST_FORCE_ITERATION: value:1	force iteration return when able
	HELICS_ITERATION_REQUEST_ITERATE_IF_NEEDED: value:2	only return an iteration if necessary
%}
classdef HelicsIterationRequest
	properties (Constant)
		HELICS_ITERATION_REQUEST_NO_ITERATION = int32(0);
		HELICS_ITERATION_REQUEST_FORCE_ITERATION = int32(1);
		HELICS_ITERATION_REQUEST_ITERATE_IF_NEEDED = int32(2);
	end
end