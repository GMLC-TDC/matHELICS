function varargout = helicsFederateRegisterGlobalInput(varargin)
% Register a global named input.
% 
% @details The publication becomes part of the federate and is destroyed when the federate is freed so there are no separate free
% functions for inputs and publications.
% 
% @param fed The federate object in which to create a publication.
% @param key The identifier for the input (may be NULL).
% @param type A code identifying the type of the input see /ref HelicsDataTypes for available options.
% @param units A string listing the units of the input (may be NULL).
% 
% 
% 
% @return An object containing the input.

	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterGlobalInput', varargin{:});
end
