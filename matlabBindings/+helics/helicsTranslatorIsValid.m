function varargout = helicsTranslatorIsValid(varargin)
% Check if a translator is valid.
% 
% @param trans The translator object to check.
% 
% @return HELICS_TRUE if the Translator object represents a valid translator.

	[varargout{1:nargout}] = helicsMex('helicsTranslatorIsValid', varargin{:});
end
