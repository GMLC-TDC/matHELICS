%{
%	Set an option value for a filter.

%	@param filt The given filter.
%	@param option The option to set /ref helics_handle_options.
%	@param value The value of the option commonly 0 for false 1 for true.

%	@param[in,out] err An error object to fill out in case of an error.

%}
function varargout = helicsFilterSetOption(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFilterSetOption', varargin{:});
end
