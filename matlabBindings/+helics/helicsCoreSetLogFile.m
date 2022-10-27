function varargout = helicsCoreSetLogFile(varargin)
%  Set the log file on a core.
% 
%	@param core The core to set the log file for.
%	@param logFileName The name of the file to log to.
% 
%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
	[varargout{1:nargout}] = helicsMex('helicsCoreSetLogFile', varargin{:});
end
