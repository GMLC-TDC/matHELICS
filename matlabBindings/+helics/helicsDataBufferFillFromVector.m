function varargout = helicsDataBufferFillFromVector(varargin)
%	convert a real vector to serialized bytes.

%	@param data The helicsDataBuffer to fill.
%	@param value The vector of doubles.
%	@return int The buffer size.
	[varargout{1:nargout}] = helicsMex('helicsDataBufferFillFromVector', varargin{:});
end
