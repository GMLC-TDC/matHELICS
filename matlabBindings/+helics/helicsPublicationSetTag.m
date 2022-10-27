function varargout = helicsPublicationSetTag(varargin)
 Set the data in a specific tag for a publication.

%	@param pub The publication object to set a tag for.
%	@param tagname The name of the tag to set.
%	@param tagvalue The string value to associate with a tag.

%	@param[in,out] err An error object to fill out in case of an error.
	[varargout{1:nargout}] = helicsMex('helicsPublicationSetTag', varargin{:});
end
