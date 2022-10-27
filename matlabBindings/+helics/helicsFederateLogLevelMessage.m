function varargout = helicsFederateLogLevelMessage(varargin)
%  Log a message through a federate.
% 
%	@param fed The federate to log the message through.
%	@param loglevel The level of the message to log see /ref helics_log_levels.
%	@param logmessage The message to put in the log.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateLogLevelMessage', varargin{:});
end
