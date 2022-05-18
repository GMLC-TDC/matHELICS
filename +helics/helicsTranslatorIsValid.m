%{
	Check if a translator is valid.

	@param trans The translator object to check.

	@return HELICS_TRUE if the Translator object represents a valid translator.
%}
function varargout = helicsTranslatorIsValid(varargin)
	[varargout{1:nargout}] = helicsMex('helicsTranslatorIsValid', varargin{:});
end
