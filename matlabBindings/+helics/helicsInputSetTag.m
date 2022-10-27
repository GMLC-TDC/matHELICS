function varargout = helicsInputSetTag(varargin)
%  Set the data in a specific tag for an input.
% 
%	@param inp The input object to query.
%	@param tagname The string to set.
%	@param tagvalue The string value to associate with a tag.
% 
	[varargout{1:nargout}] = helicsMex('helicsInputSetTag', varargin{:});
end
