%{
	Get the data in a specified tag of a translator.

	@param trans The translator to query.
	@param tagname The name of the tag to query.
	@return A string with the tag data.
%}
function varargout = helicsTranslatorGetTag(varargin)
	[varargout{1:nargout}] = helicsMex('helicsTranslatorGetTag', varargin{:});
end
