function varargout = helicsCallbackFederateNextTimeCallback(varargin)
	Set callback for the next time update=.

	@details This callback will be executed to compute the next time update for a callback federate.

	@param fed The federate object in which to set the callback.
	@param timeUpdate A function handle with the signature HelicsTime (HelicsTime time).
	[varargout{1:nargout}] = helicsMex('helicsCallbackFederateNextTimeCallback', varargin{:});
end
