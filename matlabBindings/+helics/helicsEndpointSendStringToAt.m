function varargout = helicsEndpointSendStringToAt(varargin)
% Send a message to the specified destination at a specific time.
% 
% @param endpoint The endpoint to send the data from.
% @param message The data to send.
% 
% @param dst The target destination. Use nullptr to send to the default destination.
% 
% @param time The time the message should be sent.
% 
% @param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsEndpointSendStringToAt', varargin{:});
end
