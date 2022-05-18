%{
	convert a complex to serialized bytes.

	@param value The complex value.
	@return HelicsDataBuffer.
%}
function varargout = helicsComplexToBytes(varargin)
	[varargout{1:nargout}] = helicsMex('helicsComplexToBytes', varargin{:});
end
