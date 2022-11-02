function varargout = helicsFederateGetCommand(varargin)
% Get a command sent to the federate.
% 
% @param fed The federate to get the command for.
% 
% 
% @return A string with the command for the federate, if the string is empty no command is available.

	[varargout{1:nargout}] = helicsMex('helicsFederateGetCommand', varargin{:});
end
