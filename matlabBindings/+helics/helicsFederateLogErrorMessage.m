%{
%	Log an error message through a federate.

%	@param fed The federate to log the error message through.
%	@param logmessage The message to put in the log.

%	@param[in,out] err A pointer to an error object for catching errors.
%}
function varargout = helicsFederateLogErrorMessage(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateLogErrorMessage', varargin{:});
end
