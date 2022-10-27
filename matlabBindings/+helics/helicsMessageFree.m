function varargout = helicsMessageFree(varargin)
 Free a message object from memory
%	@param message The message object to copy from.
%	@details memory for message is managed so not using this function does not create memory leaks, this is an indication
%	to the system that the memory for this message is done being used and can be reused for a new message.
%	helicsFederateClearMessages() can also be used to clear up all stored messages at once
	[varargout{1:nargout}] = helicsMex('helicsMessageFree', varargin{:});
end
