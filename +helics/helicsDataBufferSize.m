%{ get the data buffer size*/%}
function varargout = helicsDataBufferSize(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferSize', varargin{:});
end
