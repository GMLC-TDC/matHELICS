%{
	Get a complex value from an input object.

	@param ipt The input to get the data for.

	@return  A complex number.
%}
function varargout = helicsInputGetComplex(varargin)
	[varargout{1:nargout}] = helicsMex("helicsInputGetComplex", varargin{:});
end
