function varargout = helicsDataBufferFillFromComplexVector(varargin)
%	convert a complex vector to serialized bytes.

%	@param data The helicsDataBuffer to fill.
%	@param value The vector of complex values.
%	@return int The buffer size.
	[varargout{1:nargout}] = helicsMex('helicsDataBufferFillFromComplexVector', varargin{:});
end
