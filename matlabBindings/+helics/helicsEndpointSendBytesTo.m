function varargout = helicsEndpointSendBytesTo(varargin)
%	Send a message to the specified destination.

%	@param endpoint The endpoint to send the data from.
%	@param data The data to send.
%	@param dst The destination to send the message to.
	[varargout{1:nargout}] = helicsMex('helicsEndpointSendBytesTo', varargin{:});
end
