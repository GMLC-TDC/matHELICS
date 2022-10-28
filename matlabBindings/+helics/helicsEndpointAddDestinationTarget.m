function varargout = helicsEndpointAddDestinationTarget(varargin)
%  add a destination target to an endpoint,  Specifying an endpoint to send undirected messages to
% 
%	@param endpoint The endpoint to modify.
%	@param targetEndpoint the name of the endpoint to send messages to
% 

	[varargout{1:nargout}] = helicsMex('helicsEndpointAddDestinationTarget', varargin{:});
end
