%{
	convert an int to serialized bytes.

	@param value The integer.
	@return HelicsDataBuffer.
%}
function varargout = helicsIntToBytes(varargin)
	[varargout{1:nargout}] = helicsMex('helicsIntToBytes', varargin{:});
end
