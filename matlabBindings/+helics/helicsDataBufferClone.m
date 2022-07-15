%{ create a new data buffer and copy an existing buffer*/%}
function varargout = helicsDataBufferClone(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferClone', varargin{:});
end
