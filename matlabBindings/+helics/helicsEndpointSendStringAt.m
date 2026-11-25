function varargout = helicsEndpointSendStringAt(varargin)
% Send a message at a specific time to the targeted destinations.
% 
% @param endpoint The endpoint to send the data from.
% 
% @param message The data to send.
% 
% @param time The time the message should be sent.
% 
% @param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsEndpointSendStringAt', varargin{:});
end
