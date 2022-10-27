function varargout = helicsFederateLogInfoMessage(varargin)
%  Log an info message through a federate.
% 
%	@param fed The federate to log the info message through.
%	@param logmessage The message to put in the log.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateLogInfoMessage', varargin{:});
end
