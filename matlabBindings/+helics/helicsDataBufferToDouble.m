%{ convert a data buffer to a double*/%}
function varargout = helicsDataBufferToDouble(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToDouble', varargin{:});
end
