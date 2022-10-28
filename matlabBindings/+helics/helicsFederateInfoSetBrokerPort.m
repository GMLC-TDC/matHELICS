function varargout = helicsFederateInfoSetBrokerPort(varargin)
%  Set the port to use for the broker.
% 
%	@details This is only used if the core is automatically created, the broker information will be transferred to the core for connection.
%	This will only be useful for network broker connections.
%	@param fi The federate info object to alter.
%	@param brokerPort The integer port number to use for connection with a broker.
% 

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetBrokerPort', varargin{:});
end
