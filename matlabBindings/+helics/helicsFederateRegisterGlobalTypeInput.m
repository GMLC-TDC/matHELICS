function varargout = helicsFederateRegisterGlobalTypeInput(varargin)
% Register an input with an arbitrary type.
% 
% @details The input becomes part of the federate and is destroyed when the federate is freed so there are no separate free
% functions for interfaces.
% 
% @param fed The federate object in which to create an input.
% @param key The identifier for the input (may be NULL).
% @param type A string defining the type of the input (may be NULL).
% @param units A string listing the units of the input (may be NULL).
% 
% @return An object containing the input.

	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterGlobalTypeInput', varargin{:});
end
