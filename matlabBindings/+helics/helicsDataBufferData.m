%{ get a pointer to the raw data*/%}
function varargout = helicsDataBufferData(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferData', varargin{:});
end
