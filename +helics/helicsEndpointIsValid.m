%{
	Check if an endpoint is valid.

	@param endpoint The endpoint object to check.

	@return HELICS_TRUE if the Endpoint object represents a valid endpoint.
%}
function varargout = helicsEndpointIsValid(varargin)
	[varargout{1:nargout}] = helicsMex("helicsEndpointIsValid", varargin{:});
end
