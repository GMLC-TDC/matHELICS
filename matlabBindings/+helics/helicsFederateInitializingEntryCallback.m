function varargout = helicsFederateInitializingEntryCallback(varargin)
	Set callback for the entry to initializingMode.n
	@details This callback will be executed when the initializingMode is entered.

	@param fed The federate object in which to set the callback.
	@param initializingEntry A function handle with the signature void(HelicsBool iterating).
	[varargout{1:nargout}] = helicsMex('helicsFederateInitializingEntryCallback', varargin{:});
end
