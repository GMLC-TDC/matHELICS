function varargout = helicsBrokerGetAddress(varargin)
 Get the network address associated with a broker.

%	@param broker The broker to query.

%	@return A string with the network address of the broker.
	[varargout{1:nargout}] = helicsMex('helicsBrokerGetAddress', varargin{:});
end
