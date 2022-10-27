function varargout = helicsCoreSendCommand(varargin)
%  Send a command to another helics object though a core using asynchronous(fast) operations.
% 
%	@param core The core to send the command through.
%	@param target The name of the object to send the command to.
%	@param command The command to send.
% 
	[varargout{1:nargout}] = helicsMex('helicsCoreSendCommand', varargin{:});
end
