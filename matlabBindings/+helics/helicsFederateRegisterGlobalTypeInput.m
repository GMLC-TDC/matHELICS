function varargout = helicsFederateRegisterGlobalTypeInput(varargin)
%  Register a global publication with an arbitrary type.
% 
%	@details The publication becomes part of the federate and is destroyed when the federate is freed so there are no separate free
%	functions for subscriptions and publications.
% 
%	@param fed The federate object in which to create a publication.
%	@param key The identifier for the publication.
%	@param type A string defining the type of the input.
%	@param units A string listing the units of the subscription maybe NULL.
% 
%
% 
%	@return An object containing the publication.
	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterGlobalTypeInput', varargin{:});
end
