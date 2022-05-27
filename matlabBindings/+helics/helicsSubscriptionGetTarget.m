%{
	Get the target of a subscription.

	@return A const char with the subscription target.
%}
function varargout = helicsSubscriptionGetTarget(varargin)
	[varargout{1:nargout}] = helicsMex('helicsSubscriptionGetTarget', varargin{:});
end
