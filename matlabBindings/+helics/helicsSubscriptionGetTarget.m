function varargout = helicsSubscriptionGetTarget(varargin)
 Get the target of a subscription.

%	@return A const char with the subscription target.
	[varargout{1:nargout}] = helicsMex('helicsSubscriptionGetTarget', varargin{:});
end
