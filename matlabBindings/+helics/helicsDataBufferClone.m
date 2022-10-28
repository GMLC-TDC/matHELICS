function varargout = helicsDataBufferClone(varargin)
/** create a new data buffer and copy an existing buffer*/
	[varargout{1:nargout}] = helicsMex('helicsDataBufferClone', varargin{:});
end
