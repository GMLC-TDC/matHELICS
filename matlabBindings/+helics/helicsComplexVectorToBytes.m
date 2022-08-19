%{
%	convert a complex vector to serialized bytes.

%	@param value The vector of complex values.
%	@return HelicsDataBuffer.
%}
function varargout = helicsComplexVectorToBytes(varargin)
	[varargout{1:nargout}] = helicsMex('helicsComplexVectorToBytes', varargin{:});
end
