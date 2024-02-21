function varargout = helicsFederateExecutingEntryCallback(varargin)
	Set callback for the entry to ExecutingMode.n
	@details This callback will be executed once on first entry to executing mode.

	@param fed The federate object in which to set the callback.
	@param executingEntry A function handle with the signature void(void).
	[varargout{1:nargout}] = helicsMex('helicsFederateExecutingEntryCallback', varargin{:});
end
