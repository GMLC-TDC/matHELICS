%{
%	Get the data in a specified tag of a filter.

%	@param filt The filter to query.
%	@param tagname The name of the tag to query.
%	@return A string with the tag data.
%}
function varargout = helicsFilterGetTag(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFilterGetTag', varargin{:});
end
