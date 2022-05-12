%{
	Get the data in a specified tag of a publication.

	@param pub The publication object to query.
	@param tagname The name of the tag to query.
	@return A string with the tag data.
%}
function varargout = helicsPublicationGetTag(varargin)
	[varargout{1:nargout}] = helicsMex("helicsPublicationGetTag", varargin{:});
end
