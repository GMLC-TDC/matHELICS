%{
	Get the name of an endpoint.

	@param endpoint The endpoint object in question.

	@return The name of the endpoint.
%}
function varargout = helicsEndpointGetName(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointGetName', varargin{:});
end
