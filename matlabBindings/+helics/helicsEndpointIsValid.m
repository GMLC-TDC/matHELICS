function varargout = helicsEndpointIsValid(varargin)
% Check if an endpoint is valid.
% 
% @param endpoint The endpoint object to check.
% 
% @return HELICS_TRUE if the Endpoint object represents a valid endpoint.

	[varargout{1:nargout}] = helicsMex('helicsEndpointIsValid', varargin{:});
end
