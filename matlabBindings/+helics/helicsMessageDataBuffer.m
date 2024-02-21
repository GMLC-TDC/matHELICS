function varargout = helicsMessageDataBuffer(varargin)
% Get a data buffer to the message object
% 
% @param message A message object to get the dataBuffer for
% 
% @return A HelicsDataBuffer object to the data in a message.  Modifying the buffer will directly modify the message contents.

	[varargout{1:nargout}] = helicsMex('helicsMessageDataBuffer', varargin{:});
end
