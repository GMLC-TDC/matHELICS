%{
%	Get the type specified for an endpoint.

%	@param endpoint The endpoint object in question.

%	@return The defined type of the endpoint.
%}
function varargout = helicsEndpointGetType(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointGetType', varargin{:});
end
