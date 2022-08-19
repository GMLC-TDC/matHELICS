%{
%	Check if a given endpoint has any unread messages.

%	@param endpoint The endpoint to check.

%	@return HELICS_TRUE if the endpoint has a message, HELICS_FALSE otherwise.
%}
function varargout = helicsEndpointHasMessage(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointHasMessage', varargin{:});
end
