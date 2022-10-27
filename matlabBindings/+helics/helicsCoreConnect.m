function varargout = helicsCoreConnect(varargin)
 Connect a core to the federate based on current configuration.

%	@param core The core to connect.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.


%	@return HELICS_FALSE if not connected, HELICS_TRUE if it is connected.
	[varargout{1:nargout}] = helicsMex('helicsCoreConnect', varargin{:});
end
