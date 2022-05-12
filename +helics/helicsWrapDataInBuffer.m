%{ wrap user data in a buffer object*/%}
function varargout = helicsWrapDataInBuffer(varargin)
	[varargout{1:nargout}] = helicsMex("helicsWrapDataInBuffer", varargin{:});
end
