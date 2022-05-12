%{
	Get the data in a specified tag of an input.

	@param inp The input object to query.
	@param tagname The name of the tag to get the value for.
	@return A string with the tag data.
%}
function varargout = helicsInputGetTag(varargin)
	[varargout{1:nargout}] = helicsMex("helicsInputGetTag", varargin{:});
end
