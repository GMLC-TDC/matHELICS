%{
	Get the size of the raw value for subscription.

	@return The size of the raw data/string in bytes.
%}
function varargout = helicsInputGetByteCount(varargin)
	[varargout{1:nargout}] = helicsMex("helicsInputGetByteCount", varargin{:});
end
