%{
%	Get a handle option for the translator.

%	@param trans The given translator to query.
%	@param option The option to query /ref helics_handle_options.
%}
function varargout = helicsTranslatorGetOption(varargin)
	[varargout{1:nargout}] = helicsMex('helicsTranslatorGetOption', varargin{:});
end
