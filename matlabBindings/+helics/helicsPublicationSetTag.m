function varargout = helicsPublicationSetTag(varargin)
%  Set the data in a specific tag for a publication.
% 
%	@param pub The publication object to set a tag for.
%	@param tagname The name of the tag to set.
%	@param tagvalue The string value to associate with a tag.
% 
	[varargout{1:nargout}] = helicsMex('helicsPublicationSetTag', varargin{:});
end
