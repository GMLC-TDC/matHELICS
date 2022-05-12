%{
	Get the name of the translator and store in the given string.

	@param trans The given translator.

	@return A string with the name of the translator.
%}
function varargout = helicsTranslatorGetName(varargin)
	[varargout{1:nargout}] = helicsMex("helicsTranslatorGetName", varargin{:});
end
