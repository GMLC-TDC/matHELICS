%{
	convert an int to serialized bytes.

	@param value The integer.
	@return HelicsDataBuffer.
%}
function varargout = helicsIntegerToBytes(varargin)
	[varargout{1:nargout}] = helicsMex('helicsIntegerToBytes', varargin{:});
end
