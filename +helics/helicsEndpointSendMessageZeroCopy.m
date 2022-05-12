%{
	Send a message object from a specific endpoint, the message will not be copied and the message object will no longer be valid
	after the call.

	@param endpoint The endpoint to send the data from.
	@param message The actual message to send which will be copied.

	@param[in,out] err A pointer to an error object for catching errors.
%}
function varargout = helicsEndpointSendMessageZeroCopy(varargin)
	[varargout{1:nargout}] = helicsMex("helicsEndpointSendMessageZeroCopy", varargin{:});
end
