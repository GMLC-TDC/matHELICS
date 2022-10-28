function varargout = helicsFederateWaitCommand(varargin)
%  Get a command sent to the federate. Blocks until a command is received.
% 
%	@param fed The federate to get the command for.
% 
% 
%	@return A string with the command for the federate, if the string is empty no command is available.
	[varargout{1:nargout}] = helicsMex('helicsFederateWaitCommand', varargin{:});
end
