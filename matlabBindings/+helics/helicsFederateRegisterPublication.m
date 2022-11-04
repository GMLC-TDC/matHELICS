function varargout = helicsFederateRegisterPublication(varargin)
% Register a publication with a known type.
% 
% @details The publication becomes part of the federate and is destroyed when the federate is freed so there are no separate free
% functions for subscriptions and publications.
% 
% @param fed The federate object in which to create a publication.
% @param key The identifier for the publication the global publication key will be prepended with the federate name.
% @param type A code identifying the type of the input see /ref HelicsDataTypes for available options.
% @param units A string listing the units of the subscription (may be NULL).
% 
% 
% 
% @return An object containing the publication.

	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterPublication', varargin{:});
end
