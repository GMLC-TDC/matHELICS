%{ check whether a buffer is valid*/%}
function varargout = helicsDataBufferIsValid(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferIsValid', varargin{:});
end
