function varargout = helicsFederateRegisterInput(varargin)
 Register a named input.

%	@details The input becomes part of the federate and is destroyed when the federate is freed so there are no separate free
%	functions for subscriptions, inputs, and publications.

%	@param fed The federate object in which to create an input.
%	@param key The identifier for the publication the global input key will be prepended with the federate name.
%	@param type A code identifying the type of the input see /ref HelicsDataTypes for available options.
%	@param units A string listing the units of the input (may be NULL).

%	@param[in,out] err A pointer to an error object for catching errors.


%	@return An object containing the input.
	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterInput', varargin{:});
end
