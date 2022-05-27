%{
	Set a handle option on an endpoint.

	@param endpoint The endpoint to modify.
	@param option Integer code for the option to set /ref helics_handle_options.
	@param value The value to set the option to.

	@param[in,out] err An error object to fill out in case of an error.

%}
function varargout = helicsEndpointSetOption(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointSetOption', varargin{:});
end
