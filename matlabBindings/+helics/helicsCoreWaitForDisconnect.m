function varargout = helicsCoreWaitForDisconnect(varargin)
%  Wait for the core to disconnect.
% 
%	@param core The core to wait for.
%	@param msToWait The time out in millisecond (<0 for infinite timeout).
% 
%
% 
%	@return HELICS_TRUE if the disconnect was successful, HELICS_FALSE if there was a timeout.
	[varargout{1:nargout}] = helicsMex('helicsCoreWaitForDisconnect', varargin{:});
end
