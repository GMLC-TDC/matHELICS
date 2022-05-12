%{
	Get the data in a specified tag of an endpoint

	@param endpoint The endpoint to query.
	@param tagname The name of the tag to query.
	@return A string with the tag data.
%}
function varargout = helicsEndpointGetTag(varargin)
	[varargout{1:nargout}] = helicsMex("helicsEndpointGetTag", varargin{:});
end
