%{
	Get a named point from a subscription.

	@param ipt The input to get the result for.

	@return a string and a double value for the named point
%}
function varargout = helicsInputGetNamedPoint(varargin)
	[varargout{1:nargout}] = helicsMex('helicsInputGetNamedPoint', varargin{:});
end
