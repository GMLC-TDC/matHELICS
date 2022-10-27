function varargout = helicsEndpointAddDestinationFilter(varargin)
%  add a destination filter to an endpoint
% 
%	@param endpoint The endpoint to modify.
%	@param filterName The name of the filter to add.
% 
	[varargout{1:nargout}] = helicsMex('helicsEndpointAddDestinationFilter', varargin{:});
end
