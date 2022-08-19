%{
%	Log an info message through a federate.

%	@param fed The federate to log the info message through.
%	@param logmessage The message to put in the log.

%	@param[in,out] err A pointer to an error object for catching errors.
%}
function varargout = helicsFederateLogInfoMessage(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateLogInfoMessage', varargin{:});
end
