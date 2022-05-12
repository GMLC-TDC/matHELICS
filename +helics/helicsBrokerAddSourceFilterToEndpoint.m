%{
	Link a named filter to a source endpoint.

	@param broker The broker to generate the connection from.
	@param filter The name of the filter (cannot be NULL).
	@param endpoint The name of the endpoint to filter the data from (cannot be NULL).

	@param[in,out] err A HelicsError object, can be NULL if the errors are to be ignored.

%}
function varargout = helicsBrokerAddSourceFilterToEndpoint(varargin)
	[varargout{1:nargout}] = helicsMex("helicsBrokerAddSourceFilterToEndpoint", varargin{:});
end
