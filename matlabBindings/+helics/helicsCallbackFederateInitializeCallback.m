function varargout = helicsCallbackFederateInitializeCallback(varargin)
	Set callback for initialization.

	@details This callback will be executed when computing whether to iterate in initialization mode.

	@param fed The federate object in which to set the callback.
	@param initialize A function handle with the signature HelicsIterationRequest(void).
	[varargout{1:nargout}] = helicsMex('helicsCallbackFederateInitializeCallback', varargin{:});
end
