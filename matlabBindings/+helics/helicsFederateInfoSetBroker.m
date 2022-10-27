function varargout = helicsFederateInfoSetBroker(varargin)
%  Set the name or connection information for a broker.
% 
%	@details This is only used if the core is automatically created, the broker information will be transferred to the core for connection.
%	@param fi The federate info object to alter.
%	@param broker A string which defines the connection information for a broker either a name or an address.
% 

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetBroker', varargin{:});
end
