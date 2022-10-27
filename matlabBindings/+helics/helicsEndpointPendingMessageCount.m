function varargout = helicsEndpointPendingMessageCount(varargin)
 Returns the number of pending receives for all endpoints of a particular federate.

%	@param endpoint The endpoint to query.
	[varargout{1:nargout}] = helicsMex('helicsEndpointPendingMessageCount', varargin{:});
end
