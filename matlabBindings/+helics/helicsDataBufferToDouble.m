function varargout = helicsDataBufferToDouble(varargin)
/** convert a data buffer to a double*/
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToDouble', varargin{:});
end
