%{
	Get the default destination for an endpoint.

	@param endpoint The endpoint to set the destination for.

	@return A string with the default destination.
%}
function varargout = helicsEndpointGetDefaultDestination(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointGetDefaultDestination', varargin{:});
end
