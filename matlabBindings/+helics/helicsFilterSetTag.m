function varargout = helicsFilterSetTag(varargin)
% Set the data in a specific tag for a filter.
% 
% @param filt The filter object to set the tag for.
% @param tagname The string to set.
% @param tagvalue the string value to associate with a tag.
% 
% @param[in,out] err An error object to fill out in case of an error.

	[varargout{1:nargout}] = helicsMex('helicsFilterSetTag', varargin{:});
end
