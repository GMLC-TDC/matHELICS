function varargout = helicsFederateRegisterGlobalTypePublication(varargin)
% Register a global publication with a defined type.
% 
% @details The publication becomes part of the federate and is destroyed when the federate is freed so there are no separate free
% functions for inputs and publications.
% 
% @param fed The federate object in which to create a publication.
% @param key The identifier for the publication (may be NULL).
% @param type A string describing the expected type of the publication (may be NULL).
% @param units A string listing the units of the publication (may be NULL).
% 
% 
% 
% @return An object containing the publication.

	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterGlobalTypePublication', varargin{:});
end
