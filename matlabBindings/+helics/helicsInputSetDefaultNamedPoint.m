%{
	Set the default as a NamedPoint.

	@param ipt The input to set the default for.
	@param str A pointer to a string representing the name.
	@param val A double value for the value of the named point.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsInputSetDefaultNamedPoint(varargin)
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultNamedPoint', varargin{:});
end
