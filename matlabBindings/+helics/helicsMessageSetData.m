%{
	Set the data payload of a message as raw data.

	@param message The message object in question.
	@param data A string containing the message data.
%}
function varargout = helicsMessageSetData(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageSetData', varargin{:});
end
