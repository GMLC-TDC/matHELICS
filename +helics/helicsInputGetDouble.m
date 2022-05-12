%{
	Get a double value from a subscription.

	@param ipt The input to get the data for.

	@param[in,out] err A pointer to an error object for catching errors.

	@return The double value of the input.
%}
function varargout = helicsInputGetDouble(varargin)
	[varargout{1:nargout}] = helicsMex("helicsInputGetDouble", varargin{:});
end
