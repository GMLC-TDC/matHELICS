function varargout = helicsFederateRegisterGlobalInput(varargin)
 Register a global named input.

%	@details The publication becomes part of the federate and is destroyed when the federate is freed so there are no separate free
%	functions for subscriptions and publications.

%	@param fed The federate object in which to create a publication.
%	@param key The identifier for the publication.
%	@param type A code identifying the type of the input see /ref HelicsDataTypes for available options.
%	@param units A string listing the units of the subscription maybe NULL.

%	@param[in,out] err A pointer to an error object for catching errors.


%	@return An object containing the publication.
	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterGlobalInput', varargin{:});
end
