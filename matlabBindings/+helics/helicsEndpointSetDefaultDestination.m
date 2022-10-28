function varargout = helicsEndpointSetDefaultDestination(varargin)
%  Set the default destination for an endpoint if no other endpoint is given.
% 
%	@param endpoint The endpoint to set the destination for.
%	@param dst A string naming the desired default endpoint.
% 
	[varargout{1:nargout}] = helicsMex('helicsEndpointSetDefaultDestination', varargin{:});
end
