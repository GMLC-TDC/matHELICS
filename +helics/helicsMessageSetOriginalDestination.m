%{
	Set the original destination of a message.

	@param message The message object in question.
	@param dst A string containing the new original source.

	@param[in,out] err An error object to fill out in case of an error.
%}
function varargout = helicsMessageSetOriginalDestination(varargin)
	[varargout{1:nargout}] = helicsMex("helicsMessageSetOriginalDestination", varargin{:});
end
