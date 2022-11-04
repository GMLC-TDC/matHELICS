function varargout = helicsEndpointGetInfo(varargin)
% Get the local information field of an endpoint.
% 
% @param end The endpoint to query.
% 
% @return A string with the info field string.

	[varargout{1:nargout}] = helicsMex('helicsEndpointGetInfo', varargin{:});
end
