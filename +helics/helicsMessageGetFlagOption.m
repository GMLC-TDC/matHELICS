%{
	Check if a flag is set on a message.

	@param message The message object in question.
	@param flag The flag to check should be between [0,15].

	@return The flags associated with a message.
%}
function varargout = helicsMessageGetFlagOption(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageGetFlagOption', varargin{:});
end
