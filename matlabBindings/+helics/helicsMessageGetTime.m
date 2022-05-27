%{
	Get the helics time associated with a message.

	@param message The message object in question.

	@return The time associated with a message.
%}
function varargout = helicsMessageGetTime(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageGetTime', varargin{:});
end
