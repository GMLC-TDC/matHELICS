%{
%	Get an integer value from a subscription.

%	@param ipt The input to get the data for.

%	@param[in,out] err A pointer to an error object for catching errors.

%	@return An int64_t value with the current value of the input.
%}
function varargout = helicsInputGetInteger(varargin)
	[varargout{1:nargout}] = helicsMex('helicsInputGetInteger', varargin{:});
end
