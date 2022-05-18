%{
	Set the default as a complex number.

	@param ipt The input to get the data for.
	@param value The default complex value.
%}
function varargout = helicsInputSetDefaultComplex(varargin)
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultComplex', varargin{:});
end
