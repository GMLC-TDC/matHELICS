%{
	add a destination filter to an endpoint

	@param endpoint The endpoint to modify.
	@param filterName The name of the filter to add.

	@param[in,out] err An error object to fill out in case of an error.
%}
function varargout = helicsEndpointAddDestinationFilter(varargin)
	[varargout{1:nargout}] = helicsMex("helicsEndpointAddDestinationFilter", varargin{:});
end
