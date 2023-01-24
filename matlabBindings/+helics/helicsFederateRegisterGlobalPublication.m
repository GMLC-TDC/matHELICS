function varargout = helicsFederateRegisterGlobalPublication(varargin)
% Register a global named publication with an arbitrary type.
% 
% @details The publication becomes part of the federate and is destroyed when the federate is freed so there are no separate free
% functions for inputs and publications.
% 
% @param fed The federate object in which to create a publication.
% @param key The identifier for the publication (may be NULL).
% @param type A code identifying the type of the input see /ref HelicsDataTypes for available options.
% @param units A string listing the units of the publication (may be NULL).
% 
% 
% 
% @return An object containing the publication.

	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterGlobalPublication', varargin{:});
end
