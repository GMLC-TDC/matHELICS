%{
	Set the default as a string.

	@param ipt The input to set the default for.
	@param str A pointer to the default string.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsInputSetDefaultString(varargin)
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultString', varargin{:});
end
