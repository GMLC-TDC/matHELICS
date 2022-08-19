%{
%	Set the name or connection information for a broker.

%	@details This is only used if the core is automatically created, the broker information will be transferred to the core for connection.
%	@param fi The federate info object to alter.
%	@param broker A string which defines the connection information for a broker either a name or an address.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

%}
function varargout = helicsFederateInfoSetBroker(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetBroker', varargin{:});
end
