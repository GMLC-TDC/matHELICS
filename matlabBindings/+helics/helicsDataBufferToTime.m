%{ convert a data buffer to a time*/%}
function varargout = helicsDataBufferToTime(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToTime', varargin{:});
end
