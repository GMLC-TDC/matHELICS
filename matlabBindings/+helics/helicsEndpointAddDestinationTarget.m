%{
	add a destination target to an endpoint,  Specifying an endpoint to send undirected messages to

	@param endpoint The endpoint to modify.
	@param targetEndpoint the name of the endpoint to send messages to

	@param[in,out] err An error object to fill out in case of an error.

%}
function varargout = helicsEndpointAddDestinationTarget(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointAddDestinationTarget', varargin{:});
end
