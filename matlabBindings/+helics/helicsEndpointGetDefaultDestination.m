function varargout = helicsEndpointGetDefaultDestination(varargin)
% Get the default destination for an endpoint.
% 
% @param endpoint The endpoint to set the destination for.
% 
% @return A string with the default destination.

	[varargout{1:nargout}] = helicsMex('helicsEndpointGetDefaultDestination', varargin{:});
end
