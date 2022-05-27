%{ Load a signal handler that handles Ctrl-C and shuts down all HELICS brokers, cores,
and federates then exits the process.*/%}
function varargout = helicsLoadSignalHandler(varargin)
	[varargout{1:nargout}] = helicsMex('helicsLoadSignalHandler', varargin{:});
end
