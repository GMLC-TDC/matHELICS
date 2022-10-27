function varargout = helicsEndpointSendMessage(varargin)
%  Send a message object from a specific endpoint.
% 
%	@param endpoint The endpoint to send the data from.
%	@param message The actual message to send which will be copied.
% 
	[varargout{1:nargout}] = helicsMex('helicsEndpointSendMessage', varargin{:});
end
