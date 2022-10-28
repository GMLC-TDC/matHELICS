function varargout = helicsCoreSetLogFile(varargin)
%  Set the log file on a core.
% 
%	@param core The core to set the log file for.
%	@param logFileName The name of the file to log to.
% 
	[varargout{1:nargout}] = helicsMex('helicsCoreSetLogFile', varargin{:});
end
