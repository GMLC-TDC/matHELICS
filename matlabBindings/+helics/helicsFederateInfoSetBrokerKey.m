function varargout = helicsFederateInfoSetBrokerKey(varargin)
%  Set the key for a broker connection.
% 
%	@details This is only used if the core is automatically created, the broker information will be transferred to the core for connection.
%	@param fi The federate info object to alter.
%	@param brokerkey A string containing a key for the broker to connect.
% 

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetBrokerKey', varargin{:});
end
