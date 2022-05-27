%{ increase the capacity a data buffer can hold without reallocating memory*/%}
function varargout = helicsDataBufferReserve(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferReserve', varargin{:});
end
