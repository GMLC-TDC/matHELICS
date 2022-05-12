%{
	Clear the flags of a message.

	@param message The message object in question
%}
function varargout = helicsMessageClearFlags(varargin)
	[varargout{1:nargout}] = helicsMex("helicsMessageClearFlags", varargin{:});
end
