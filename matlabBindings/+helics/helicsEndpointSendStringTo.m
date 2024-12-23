function varargout = helicsEndpointSendStringTo(varargin)
% Send a message to the specified destination.
% 
% @param endpoint The endpoint to send the data from.
% 
% @param message The string to send.
% 
% @param dst The target destination. Use nullptr to send to the default destination.
% @param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsEndpointSendStringTo', varargin{:});
end
