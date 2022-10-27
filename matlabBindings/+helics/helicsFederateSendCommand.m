function varargout = helicsFederateSendCommand(varargin)
 Send a command to another helics object through a federate.

%	@param fed The federate to send the command through.
%	@param target The name of the object to send the command to.
%	@param command The command to send.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
	[varargout{1:nargout}] = helicsMex('helicsFederateSendCommand', varargin{:});
end
