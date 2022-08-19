%{
%	Send a message to the targeted destinations at a specified time.

%	@param endpoint The endpoint to send the data from.
%	@param data The data to send.
%	@param time The time to send the message at.
%}
function varargout = helicsEndpointSendBytesAt(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointSendBytesAt', varargin{:});
end
