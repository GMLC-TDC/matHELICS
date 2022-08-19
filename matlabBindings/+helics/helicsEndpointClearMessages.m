%{
%	Clear all stored messages stored from an endpoint.

%	@details This clears messages retrieved through helicsEndpointGetMessage or helicsEndpointCreateMessage

%	@param endpoint The endpoint to clear the message for.
%}
function varargout = helicsEndpointClearMessages(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointClearMessages', varargin{:});
end
