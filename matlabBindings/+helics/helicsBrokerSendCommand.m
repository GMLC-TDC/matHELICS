function varargout = helicsBrokerSendCommand(varargin)
%  Send a command to another helics object through a broker using asynchronous(fast) messages.
% 
%	@param broker The broker to send the command through.
%	@param target The name of the object to send the command to.
%	@param command The command to send.
% 
	[varargout{1:nargout}] = helicsMex('helicsBrokerSendCommand', varargin{:});
end
