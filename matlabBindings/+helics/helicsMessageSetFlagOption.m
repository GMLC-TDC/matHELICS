%{
	Set a flag on a message.

	@param message The message object in question.
	@param flag An index of a flag to set on the message.
	@param flagValue The desired value of the flag.

	@param[in,out] err An error object to fill out in case of an error.
%}
function varargout = helicsMessageSetFlagOption(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageSetFlagOption', varargin{:});
end
