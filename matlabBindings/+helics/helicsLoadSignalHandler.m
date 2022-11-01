function varargout = helicsLoadSignalHandler(varargin)
% Load a signal handler that handles Ctrl-C and shuts down all HELICS brokers, cores,
% and federates then exits the process.

	[varargout{1:nargout}] = helicsMex('helicsLoadSignalHandler', varargin{:});
end
