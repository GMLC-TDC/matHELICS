function varargout = helicsFederateCosimulationTerminationCallback(varargin)
	Set callback for cosimulation termination.n
	@details This callback will be executed once when the time advance of the federate/co-simulation has terminated
	this may be called as part of the finalize operation, or when a maxTime signal is returned from the requestTime or when and error is encountered.

	@param fed The federate object in which to set the callback.
	@param cosimTermination A function handle with the signature void(void).
	[varargout{1:nargout}] = helicsMex('helicsFederateCosimulationTerminationCallback', varargin{:});
end
