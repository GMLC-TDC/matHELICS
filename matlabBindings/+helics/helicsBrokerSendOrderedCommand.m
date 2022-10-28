function varargout = helicsBrokerSendOrderedCommand(varargin)
%  Send a command to another helics object through a broker using ordered sequencing.
% 
%	@param broker The broker to send the command through.
%	@param target The name of the object to send the command to.
%	@param command The command to send.
% 
	[varargout{1:nargout}] = helicsMex('helicsBrokerSendOrderedCommand', varargin{:});
end
