%{
	Get the size of a value for subscription assuming return as a string.

	@return The size of the string.
%}
function varargout = helicsInputGetStringSize(varargin)
	[varargout{1:nargout}] = helicsMex('helicsInputGetStringSize', varargin{:});
end
