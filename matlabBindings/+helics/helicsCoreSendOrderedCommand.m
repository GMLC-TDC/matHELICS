%{
	Send a command to another helics object though a core using ordered operations.

	@param core The core to send the command through.
	@param target The name of the object to send the command to.
	@param command The command to send.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsCoreSendOrderedCommand(varargin)
	[varargout{1:nargout}] = helicsMex('helicsCoreSendOrderedCommand', varargin{:});
end
