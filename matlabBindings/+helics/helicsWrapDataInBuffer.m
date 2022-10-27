function varargout = helicsWrapDataInBuffer(varargin)
/** wrap user data in a buffer object*/
	[varargout{1:nargout}] = helicsMex('helicsWrapDataInBuffer', varargin{:});
end
