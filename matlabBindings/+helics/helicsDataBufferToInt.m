%{ convert a data buffer to an int*/%}
function varargout = helicsDataBufferToInt(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToInt', varargin{:});
end
