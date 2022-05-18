%{
	Get the key of an input.

	@param ipt The input to query.

	@return A const char with the input name.
%}
function varargout = helicsInputGetName(varargin)
	[varargout{1:nargout}] = helicsMex('helicsInputGetName', varargin{:});
end
