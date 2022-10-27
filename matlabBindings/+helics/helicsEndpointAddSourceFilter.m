function varargout = helicsEndpointAddSourceFilter(varargin)
 add a source Filter to an endpoint

%	@param endpoint The endpoint to modify.
%	@param filterName the name of the filter to add

%	@param[in,out] err An error object to fill out in case of an error.

	[varargout{1:nargout}] = helicsMex('helicsEndpointAddSourceFilter', varargin{:});
end
