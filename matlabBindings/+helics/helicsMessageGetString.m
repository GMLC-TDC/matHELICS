%{
%	Get the payload of a message as a string.

%	@param message The message object in question.

%	@return A string representing the payload of a message.
%}
function varargout = helicsMessageGetString(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageGetString', varargin{:});
end
