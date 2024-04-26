function varargout = helicsFederateErrorHandlerCallback(varargin)
	Set callback for error handling.n
	@details This callback will be executed when a federate error is encountered

	@param fed The federate object in which to set the callback.
	@param errorHandler A function handle with the signature void(int errorCode, const char* errorString).
	[varargout{1:nargout}] = helicsMex('helicsFederateErrorHandlerCallback', varargin{:});
end
