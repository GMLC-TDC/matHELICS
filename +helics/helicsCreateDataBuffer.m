%{ create a helics managed data buffer with initial capacity*/%}
function varargout = helicsCreateDataBuffer(varargin)
	[varargout{1:nargout}] = helicsMex("helicsCreateDataBuffer", varargin{:});
end
