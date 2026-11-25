function varargout = helicsBrokerIsOpenToNewFederates(varargin)
% Check if a broker can accept new federates.
% 
% @param broker The broker to query.
% 
% @return HELICS_TRUE if the broker is open to new federates, HELICS_FALSE otherwise.

	[varargout{1:nargout}] = helicsMex('helicsBrokerIsOpenToNewFederates', varargin{:});
end
