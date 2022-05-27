%{
	Get the original source endpoint of a message, the source may have been modified by filters or other actions.

	@param message The message object in question.

	@return A string with the source of a message.
%}
function varargout = helicsMessageGetOriginalSource(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageGetOriginalSource', varargin{:});
end
