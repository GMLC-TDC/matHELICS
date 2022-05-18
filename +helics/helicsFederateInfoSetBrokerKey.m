%{
	Set the key for a broker connection.

	@details This is only used if the core is automatically created, the broker information will be transferred to the core for connection.
	@param fi The federate info object to alter.
	@param brokerkey A string containing a key for the broker to connect.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

%}
function varargout = helicsFederateInfoSetBrokerKey(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetBrokerKey', varargin{:});
end
