function varargout = helicsEndpointRemoveTarget(varargin)
%  remove an endpoint from being targeted
% 
%	@param endpoint The endpoint to modify.
%	@param targetEndpoint the name of the endpoint to send messages to
% 
%	@param[in,out] err An error object to fill out in case of an error.

	[varargout{1:nargout}] = helicsMex('helicsEndpointRemoveTarget', varargin{:});
end
