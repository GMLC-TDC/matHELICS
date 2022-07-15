%{ convert a data buffer to a char*/%}
function varargout = helicsDataBufferToChar(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToChar', varargin{:});
end
