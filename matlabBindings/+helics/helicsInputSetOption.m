function varargout = helicsInputSetOption(varargin)
 Set an option on an input

%	@param inp The input to query.
%	@param option The option to set for the input /ref helics_handle_options.
%	@param value The value to set the option to.

%	@param[in,out] err An error object to fill out in case of an error.
	[varargout{1:nargout}] = helicsMex('helicsInputSetOption', varargin{:});
end
