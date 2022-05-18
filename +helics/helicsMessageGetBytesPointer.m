%{
	Get a pointer to the raw data of a message.

	@param message A message object to get the data for.

	@return A pointer to the raw data in memory, the pointer may be NULL if the message is not a valid message.
%}
function varargout = helicsMessageGetBytesPointer(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageGetBytesPointer', varargin{:});
end
