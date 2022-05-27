%{
	Set the default destination for an endpoint if no other endpoint is given.

	@param endpoint The endpoint to set the destination for.
	@param dst A string naming the desired default endpoint.

	@param[in,out] err A pointer to an error object for catching errors.
%}
function varargout = helicsEndpointSetDefaultDestination(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointSetDefaultDestination', varargin{:});
end
