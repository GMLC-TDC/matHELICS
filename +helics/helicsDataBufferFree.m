%{ free a DataBuffer/%}
function varargout = helicsDataBufferFree(varargin)
	[varargout{1:nargout}] = helicsMex("helicsDataBufferFree", varargin{:});
end
