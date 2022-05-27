%{
	Log a debug message through a federate.

	@param fed The federate to log the debug message through.
	@param logmessage The message to put in the log.

	@param[in,out] err A pointer to an error object for catching errors.
%}
function varargout = helicsFederateLogDebugMessage(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateLogDebugMessage', varargin{:});
end
