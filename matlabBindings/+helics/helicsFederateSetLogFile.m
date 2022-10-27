function varargout = helicsFederateSetLogFile(varargin)
%  Set the logging file for a federate (actually on the core associated with a federate).
% 
%	@param fed The federate to set the log file for.
%	@param logFile The name of the log file.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateSetLogFile', varargin{:});
end
