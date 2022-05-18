%{ Clear HELICS based signal handlers.*/%}
function varargout = helicsClearSignalHandler(varargin)
	[varargout{1:nargout}] = helicsMex('helicsClearSignalHandler', varargin{:});
end
