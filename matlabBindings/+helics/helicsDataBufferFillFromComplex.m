function varargout = helicsDataBufferFillFromComplex(varargin)
%	convert a complex to serialized bytes.

%	@param data The helicsDataBuffer to fill.
%	@param value The complex value.
%	@return int The buffer size.
	[varargout{1:nargout}] = helicsMex('helicsDataBufferFillFromComplex', varargin{:});
end
