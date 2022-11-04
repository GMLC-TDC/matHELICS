function varargout = helicsDataBufferType(varargin)
% extract the data type from the data buffer, if the type isn't recognized UNKNOWN is returned

	[varargout{1:nargout}] = helicsMex('helicsDataBufferType', varargin{:});
end
