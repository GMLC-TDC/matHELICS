%{ convert a data buffer to an int*/%}
function varargout = helicsDataBufferToInteger(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToInteger', varargin{:});
end
