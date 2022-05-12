%{
	Get a compelx vector from a subscription.

	@param ipt The input to get the vector for.

	@return a list of complex values.
%}
function varargout = helicsInputGetComplexVector(varargin)
	[varargout{1:nargout}] = helicsMex("helicsInputGetComplexVector", varargin{:});
end
