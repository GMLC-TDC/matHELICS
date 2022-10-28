function varargout = helicsBrokerSetLogFile(varargin)
%  Set the log file on a broker.
% 
%	@param broker The broker to set the log file for.
%	@param logFileName The name of the file to log to.
% 
	[varargout{1:nargout}] = helicsMex('helicsBrokerSetLogFile', varargin{:});
end
