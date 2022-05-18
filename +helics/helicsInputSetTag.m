%{
	Set the data in a specific tag for an input.

	@param inp The input object to query.
	@param tagname The string to set.
	@param tagvalue The string value to associate with a tag.

	@param[in,out] err An error object to fill out in case of an error.
%}
function varargout = helicsInputSetTag(varargin)
	[varargout{1:nargout}] = helicsMex('helicsInputSetTag', varargin{:});
end
