%{
%	Set the default as a char.

%	@param ipt The input to set the default for.
%	@param val The default char value.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsInputSetDefaultChar(varargin)
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultChar', varargin{:});
end
