function varargout = helicsEndpointSendMessage(varargin)
% Send a message object from a specific endpoint.
% 
% @param endpoint The endpoint to send the data from.
% @param message The actual message to send which will be copied.
% 
% @param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsEndpointSendMessage', varargin{:});
end
