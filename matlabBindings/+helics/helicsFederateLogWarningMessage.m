%{
%	Log a warning message through a federate.

%	@param fed The federate to log the warning message through.
%	@param logmessage The message to put in the log.

%	@param[in,out] err A pointer to an error object for catching errors.
%}
function varargout = helicsFederateLogWarningMessage(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateLogWarningMessage', varargin{:});
end
