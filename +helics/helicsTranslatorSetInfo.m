%{
	Set the data in the info field for a translator.

	@param trans The given translator.
	@param info The string to set.

	@param[in,out] err An error object to fill out in case of an error.

%}
function varargout = helicsTranslatorSetInfo(varargin)
	[varargout{1:nargout}] = helicsMex('helicsTranslatorSetInfo', varargin{:});
end
