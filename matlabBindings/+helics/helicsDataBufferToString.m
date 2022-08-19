%{
%	Get a string value from a HelicsDataBuffer.

%	@param data The HelicsDataBuffer to get the string from.

%	@return the string value.
%}
function varargout = helicsDataBufferToString(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToString', varargin{:});
end
