function varargout = helicsCoreConnect(varargin)
% Connect a core to the federate based on current configuration.
% 
% @param core The core to connect.
% 
% 
% 
% @return HELICS_FALSE if not connected, HELICS_TRUE if it is connected.

	[varargout{1:nargout}] = helicsMex('helicsCoreConnect', varargin{:});
end
