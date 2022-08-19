%{
%	Get the raw data for the latest value of a subscription.

%	@param ipt The input to get the data for.

%	@return  raw Bytes of the value, the value is uninterpreted raw bytes.
%}
function varargout = helicsInputGetBytes(varargin)
	[varargout{1:nargout}] = helicsMex('helicsInputGetBytes', varargin{:});
end
