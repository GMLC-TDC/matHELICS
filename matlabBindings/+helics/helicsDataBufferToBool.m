%{ convert a data buffer to a boolean*/%}
function varargout = helicsDataBufferToBool(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToBool', varargin{:});
end
