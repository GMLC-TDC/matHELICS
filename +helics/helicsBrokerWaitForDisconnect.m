%{
	Wait for the broker to disconnect.

	@param broker The broker to wait for.
	@param msToWait The time out in millisecond (<0 for infinite timeout).

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.


	@return HELICS_TRUE if the disconnect was successful, HELICS_FALSE if there was a timeout.
%}
function varargout = helicsBrokerWaitForDisconnect(varargin)
	[varargout{1:nargout}] = helicsMex("helicsBrokerWaitForDisconnect", varargin{:});
end
