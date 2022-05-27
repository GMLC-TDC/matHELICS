%{
	convert a double to serialized bytes.

	@param value The double.
	@return HelicsDataBuffer.
%}
function varargout = helicsDoubleToBytes(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDoubleToBytes', varargin{:});
end
