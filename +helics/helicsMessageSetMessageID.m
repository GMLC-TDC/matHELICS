%{
	Set the message ID for the message.

	@details Normally this is not needed and the core of HELICS will adjust as needed.

	@param message The message object in question.
	@param messageID A new message ID.

	@param[in,out] err An error object to fill out in case of an error.
%}
function varargout = helicsMessageSetMessageID(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageSetMessageID', varargin{:});
end
