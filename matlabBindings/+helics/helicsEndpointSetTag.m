function varargout = helicsEndpointSetTag(varargin)
%  Set the data in a specific tag for an endpoint.
% 
%	@param endpoint The endpoint to query.
%	@param tagname The string to set.
%	@param tagvalue The string value to associate with a tag.
% 
%	@param[in,out] err An error object to fill out in case of an error.

	[varargout{1:nargout}] = helicsMex('helicsEndpointSetTag', varargin{:});
end
