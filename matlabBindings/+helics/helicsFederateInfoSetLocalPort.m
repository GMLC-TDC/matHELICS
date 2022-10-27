function varargout = helicsFederateInfoSetLocalPort(varargin)
 Set the local port to use.

%	@details This is only used if the core is automatically created, the port information will be transferred to the core for connection.
%	@param fi The federate info object to alter.
%	@param localPort A string with the port information to use as the local server port can be a number or "auto" or "os_local".

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetLocalPort', varargin{:});
end
