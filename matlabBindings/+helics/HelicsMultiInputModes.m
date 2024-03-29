% enumeration of the multi_input operations
%
% Attributes:
%	HELICS_MULTI_INPUT_NO_OP: value:0	time and priority order the inputs from the core library
%	HELICS_MULTI_INPUT_VECTORIZE_OPERATION: value:1	vectorize the inputs either double vector or string vector
%	HELICS_MULTI_INPUT_AND_OPERATION: value:2	all inputs are assumed to be boolean and all must be true to return true
%	HELICS_MULTI_INPUT_OR_OPERATION: value:3	all inputs are assumed to be boolean and at least one must be true to return true
%	HELICS_MULTI_INPUT_SUM_OPERATION: value:4	sum all the inputs
%	HELICS_MULTI_INPUT_DIFF_OPERATION: value:5	do a difference operation on the inputs, first-sum(rest) for double input, vector diff for vector input
%	HELICS_MULTI_INPUT_MAX_OPERATION: value:6	find the max of the inputs
%	HELICS_MULTI_INPUT_MIN_OPERATION: value:7	find the min of the inputs
%	HELICS_MULTI_INPUT_AVERAGE_OPERATION: value:8	take the average of the inputs
classdef HelicsMultiInputModes
	properties (Constant)
		HELICS_MULTI_INPUT_NO_OP = int32(0);
		HELICS_MULTI_INPUT_VECTORIZE_OPERATION = int32(1);
		HELICS_MULTI_INPUT_AND_OPERATION = int32(2);
		HELICS_MULTI_INPUT_OR_OPERATION = int32(3);
		HELICS_MULTI_INPUT_SUM_OPERATION = int32(4);
		HELICS_MULTI_INPUT_DIFF_OPERATION = int32(5);
		HELICS_MULTI_INPUT_MAX_OPERATION = int32(6);
		HELICS_MULTI_INPUT_MIN_OPERATION = int32(7);
		HELICS_MULTI_INPUT_AVERAGE_OPERATION = int32(8);
	end
end