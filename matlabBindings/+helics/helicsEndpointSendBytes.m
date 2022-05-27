%{
	Send a message to the targeted destinations.

	@param endpoint The endpoint to send the data from.
	@param data The data to send.
%}
function varargout = helicsEndpointSendBytes(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointSendBytes', varargin{:});
end
