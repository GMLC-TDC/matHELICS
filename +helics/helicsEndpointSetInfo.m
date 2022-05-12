%{
	Set the data in the interface information field for an endpoint.

	@param endpoint The endpoint to set the information for
	@param info The string to store in the field

	@param[in,out] err An error object to fill out in case of an error.

%}
function varargout = helicsEndpointSetInfo(varargin)
	[varargout{1:nargout}] = helicsMex("helicsEndpointSetInfo", varargin{:});
end
