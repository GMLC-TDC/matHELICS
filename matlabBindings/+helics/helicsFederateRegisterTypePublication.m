%{
%	Register a publication with a defined type.

%	@details The publication becomes part of the federate and is destroyed when the federate is freed so there are no separate free
%	functions for subscriptions and publications.

%	@param fed The federate object in which to create a publication.
%	@param key The identifier for the publication.
%	@param type A string labeling the type of the publication.
%	@param units A string listing the units of the subscription (may be NULL).

%	@param[in,out] err A pointer to an error object for catching errors.


%	@return An object containing the publication.
%}
function varargout = helicsFederateRegisterTypePublication(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterTypePublication', varargin{:});
end
