function varargout = helicsLoadThreadedSignalHandler(varargin)
/** Load a signal handler that handles Ctrl-C and shuts down all HELICS brokers, cores,
and federates then exits the process.  This operation will execute in a newly created and detached thread returning control back to the
calling program before completing operations.*/
	[varargout{1:nargout}] = helicsMex('helicsLoadThreadedSignalHandler', varargin{:});
end
