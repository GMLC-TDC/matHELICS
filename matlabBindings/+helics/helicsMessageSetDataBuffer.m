function varargout = helicsMessageSetDataBuffer(varargin)
% Set the data payload of a message from a HelicsDataBuffer Object
% 
% @param message The message object in question.
% @param data the dataBuffer containing the appropriate data, if null will clear the message payload
% 
% @param[in,out] err An error object to fill out in case of an error.

	[varargout{1:nargout}] = helicsMex('helicsMessageSetDataBuffer', varargin{:});
end
