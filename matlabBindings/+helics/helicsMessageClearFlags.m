function varargout = helicsMessageClearFlags(varargin)
% Clear the flags of a message.
% 
% @param message The message object in question

	[varargout{1:nargout}] = helicsMex('helicsMessageClearFlags', varargin{:});
end
