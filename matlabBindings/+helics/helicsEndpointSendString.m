function varargout = helicsEndpointSendString(varargin)
% Send a message to the targeted destination.
% 
% @param endpoint The endpoint to send the data from.
% @param message The string to send.
% @param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsEndpointSendString', varargin{:});
end
