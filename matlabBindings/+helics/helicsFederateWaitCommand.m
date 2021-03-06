%{
	Get a command sent to the federate. Blocks until a command is received.

	@param fed The federate to get the command for.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	@return A string with the command for the federate, if the string is empty no command is available.
%}
function varargout = helicsFederateWaitCommand(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateWaitCommand', varargin{:});
end
