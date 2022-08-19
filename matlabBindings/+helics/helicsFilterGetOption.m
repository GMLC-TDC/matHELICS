%{
%	Get a handle option for the filter.

%	@param filt The given filter to query.
%	@param option The option to query /ref helics_handle_options.
%}
function varargout = helicsFilterGetOption(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFilterGetOption', varargin{:});
end
