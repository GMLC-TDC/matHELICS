%{
%	convert a complex to serialized bytes.

%	@param value The complex value.
%	@return HelicsDataBuffer.
%}
function varargout = helicsComplexObjectToBytes(varargin)
	[varargout{1:nargout}] = helicsMex('helicsComplexObjectToBytes', varargin{:});
end
