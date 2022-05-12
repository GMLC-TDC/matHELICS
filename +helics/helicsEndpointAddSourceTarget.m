%{
	add a source target to an endpoint,  Specifying an endpoint to receive undirected messages from

	@param endpoint The endpoint to modify.
	@param targetEndpoint the endpoint to get messages from

	@param[in,out] err An error object to fill out in case of an error.

%}
function varargout = helicsEndpointAddSourceTarget(varargin)
	[varargout{1:nargout}] = helicsMex("helicsEndpointAddSourceTarget", varargin{:});
end
