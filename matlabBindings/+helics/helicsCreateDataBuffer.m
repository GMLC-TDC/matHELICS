function varargout = helicsCreateDataBuffer(varargin)
/** create a helics managed data buffer with initial capacity*/
	[varargout{1:nargout}] = helicsMex('helicsCreateDataBuffer', varargin{:});
end
