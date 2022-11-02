function varargout = helicsEndpointSendBytesToAt(varargin)
%	Send a message to the specified destination at a specified time.

%	@param endpoint The endpoint to send the data from.
%	@param data The data to send.
%	@param dst The destination to send the message to.
%	@param time The time to send the message at.
	[varargout{1:nargout}] = helicsMex('helicsEndpointSendBytesToAt', varargin{:});
end
