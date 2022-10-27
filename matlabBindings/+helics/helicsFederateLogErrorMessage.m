function varargout = helicsFederateLogErrorMessage(varargin)
%  Log an error message through a federate.
% 
%	@param fed The federate to log the error message through.
%	@param logmessage The message to put in the log.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateLogErrorMessage', varargin{:});
end
