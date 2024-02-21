function varargout = helicsCallbackFederateNextTimeIterativeCallback(varargin)
	Set callback for the next time update with iteration capability.

	@details This callback will be executed to compute the next time update for a callback federate.

	@param fed The federate object in which to set the callback.
	@param timeUpdate A function handle with the signature void(HelicsTime time, HelicsIterationResult iterationResult, HelicsIterationRequest* iteration).
	[varargout{1:nargout}] = helicsMex('helicsCallbackFederateNextTimeIterativeCallback', varargin{:});
end
