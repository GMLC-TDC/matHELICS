function varargout = helicsFilterGetOption(varargin)
 Get a handle option for the filter.

%	@param filt The given filter to query.
%	@param option The option to query /ref helics_handle_options.
	[varargout{1:nargout}] = helicsMex('helicsFilterGetOption', varargin{:});
end
