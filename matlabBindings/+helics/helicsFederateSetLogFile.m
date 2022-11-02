function varargout = helicsFederateSetLogFile(varargin)
% Set the logging file for a federate (actually on the core associated with a federate).
% 
% @param fed The federate to set the log file for.
% @param logFile The name of the log file.
% 
% @param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsFederateSetLogFile', varargin{:});
end
