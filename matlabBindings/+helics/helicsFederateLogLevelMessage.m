%{
%	Log a message through a federate.

%	@param fed The federate to log the message through.
%	@param loglevel The level of the message to log see /ref helics_log_levels.
%	@param logmessage The message to put in the log.

%	@param[in,out] err A pointer to an error object for catching errors.
%}
function varargout = helicsFederateLogLevelMessage(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateLogLevelMessage', varargin{:});
end
