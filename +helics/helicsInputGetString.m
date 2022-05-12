%{
	Get a string value from a subscription.

	@param ipt The input to get the string for.

	@return the string value.
%}
function varargout = helicsInputGetString(varargin)
	[varargout{1:nargout}] = helicsMex("helicsInputGetString", varargin{:});
end
