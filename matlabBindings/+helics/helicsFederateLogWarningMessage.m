function varargout = helicsFederateLogWarningMessage(varargin)
%  Log a warning message through a federate.
% 
%	@param fed The federate to log the warning message through.
%	@param logmessage The message to put in the log.
% 
%	@param[in,out] err A pointer to an error object for catching errors.
	[varargout{1:nargout}] = helicsMex('helicsFederateLogWarningMessage', varargin{:});
end
