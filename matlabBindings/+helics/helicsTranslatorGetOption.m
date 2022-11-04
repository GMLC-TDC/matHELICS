function varargout = helicsTranslatorGetOption(varargin)
% Get a handle option for the translator.
% 
% @param trans The given translator to query.
% @param option The option to query /ref helics_handle_options.

	[varargout{1:nargout}] = helicsMex('helicsTranslatorGetOption', varargin{:});
end
