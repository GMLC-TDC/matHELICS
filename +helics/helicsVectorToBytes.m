%{
	convert a real vector to serialized bytes.

	@param value The vector of doubles.
	@return HelicsDataBuffer.
%}
function varargout = helicsVectorToBytes(varargin)
	[varargout{1:nargout}] = helicsMex('helicsVectorToBytes', varargin{:});
end
