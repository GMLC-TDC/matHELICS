function varargout = helicsFederateSendCommand(varargin)
%  Send a command to another helics object through a federate.
% 
%	@param fed The federate to send the command through.
%	@param target The name of the object to send the command to.
%	@param command The command to send.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateSendCommand', varargin{:});
end
