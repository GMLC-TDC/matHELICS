function varargout = helicsEndpointSetOption(varargin)
%  Set a handle option on an endpoint.
% 
%	@param endpoint The endpoint to modify.
%	@param option Integer code for the option to set /ref helics_handle_options.
%	@param value The value to set the option to.
% 

	[varargout{1:nargout}] = helicsMex('helicsEndpointSetOption', varargin{:});
end
