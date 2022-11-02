function varargout = helicsComplexToBytes(varargin)
%	convert a complex to serialized bytes.

%	@param value The complex value.
%	@return HelicsDataBuffer.
	[varargout{1:nargout}] = helicsMex('helicsComplexToBytes', varargin{:});
end
