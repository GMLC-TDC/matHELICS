function varargout = helicsMessageGetMessageID(varargin)
 Get the messageID of a message.

%	@param message The message object in question.

%	@return The messageID.
	[varargout{1:nargout}] = helicsMex('helicsMessageGetMessageID', varargin{:});
end
