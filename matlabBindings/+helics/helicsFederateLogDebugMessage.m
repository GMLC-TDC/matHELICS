function varargout = helicsFederateLogDebugMessage(varargin)
%  Log a debug message through a federate.
% 
%	@param fed The federate to log the debug message through.
%	@param logmessage The message to put in the log.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateLogDebugMessage', varargin{:});
end
