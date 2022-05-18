%{
	Returns the number of pending receives for all endpoints of a particular federate.

	@param endpoint The endpoint to query.
%}
function varargout = helicsEndpointPendingMessageCount(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointPendingMessageCount', varargin{:});
end
