function varargout = helicsDataBufferClone(varargin)
% create a new data buffer and copy an existing buffer
% @return a data buffer object with a copy of the data

	[varargout{1:nargout}] = helicsMex('helicsDataBufferClone', varargin{:});
end
