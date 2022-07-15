%{
	convert a raw string to serialized bytes.

	@param str The string.
	@return HelicsDataBuffer.
%}
function varargout = helicsRawStringToBytes(varargin)
	[varargout{1:nargout}] = helicsMex('helicsRawStringToBytes', varargin{:});
end
