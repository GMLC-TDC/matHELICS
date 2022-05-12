%{ get the data buffer capacity*/%}
function varargout = helicsDataBufferCapacity(varargin)
	[varargout{1:nargout}] = helicsMex("helicsDataBufferCapacity", varargin{:});
end
