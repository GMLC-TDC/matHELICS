function varargout = helicsDataBufferData(varargin)
% get a pointer to the raw data

	[varargout{1:nargout}] = helicsMex('helicsDataBufferData', varargin{:});
end
