function varargout = helicsFederateGetCommand(varargin)
 Get a command sent to the federate.

%	@param fed The federate to get the command for.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

%	@return A string with the command for the federate, if the string is empty no command is available.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetCommand', varargin{:});
end
