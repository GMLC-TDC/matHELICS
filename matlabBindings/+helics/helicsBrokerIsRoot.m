function varargout = helicsBrokerIsRoot(varargin)
% Check if a broker is configured as a root broker.
% 
% @param broker The broker to query.
% 
% @return HELICS_TRUE if the broker is a root broker, HELICS_FALSE otherwise.

	[varargout{1:nargout}] = helicsMex('helicsBrokerIsRoot', varargin{:});
end
