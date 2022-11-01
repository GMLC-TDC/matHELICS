function varargout = helicsMessageSetTime(varargin)
% Set the delivery time for a message.
% 
% @param message The message object in question.
% @param time The time the message should be delivered.
% 
% @param[in,out] err An error object to fill out in case of an error.

	[varargout{1:nargout}] = helicsMex('helicsMessageSetTime', varargin{:});
end
