%{ convert a data buffer to a boolean*/%}
function varargout = helicsDataBufferToBoolean(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToBoolean', varargin{:});
end
