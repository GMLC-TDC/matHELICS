function varargout = helicsEndpointAddSourceTarget(varargin)
%  add a source target to an endpoint,  Specifying an endpoint to receive undirected messages from
% 
%	@param endpoint The endpoint to modify.
%	@param targetEndpoint the endpoint to get messages from
% 

	[varargout{1:nargout}] = helicsMex('helicsEndpointAddSourceTarget', varargin{:});
end
