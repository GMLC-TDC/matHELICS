function varargout = helicsFederateRegisterSubscription(varargin)
%  Create a subscription.
% 
%	@details The subscription becomes part of the federate and is destroyed when the federate is freed so there are no separate free
%	functions for subscriptions and publications.
% 
%	@param fed The federate object in which to create a subscription, must have been created with /ref helicsCreateValueFederate or
%	/ref helicsCreateCombinationFederate.
%	@param key The identifier matching a publication to get a subscription for.
%	@param units A string listing the units of the subscription (may be NULL).
% 
%
% 
%	@return An object containing the subscription.
	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterSubscription', varargin{:});
end
