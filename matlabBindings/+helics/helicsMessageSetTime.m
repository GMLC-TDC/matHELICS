function varargout = helicsMessageSetTime(varargin)
%  Set the delivery time for a message.
% 
%	@param message The message object in question.
%	@param time The time the message should be delivered.
% 
	[varargout{1:nargout}] = helicsMex('helicsMessageSetTime', varargin{:});
end
