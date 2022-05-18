%{
	Set a handle option on an endpoint.

	@param endpoint The endpoint to modify.
	@param option Integer code for the option to set /ref helics_handle_options.
	@return the value of the option, for boolean options will be 0 or 1
%}
function varargout = helicsEndpointGetOption(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointGetOption', varargin{:});
end
