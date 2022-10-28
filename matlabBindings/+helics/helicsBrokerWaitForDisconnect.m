function varargout = helicsBrokerWaitForDisconnect(varargin)
%  Wait for the broker to disconnect.
% 
%	@param broker The broker to wait for.
%	@param msToWait The time out in millisecond (<0 for infinite timeout).
% 
%
% 
%	@return HELICS_TRUE if the disconnect was successful, HELICS_FALSE if there was a timeout.
	[varargout{1:nargout}] = helicsMex('helicsBrokerWaitForDisconnect', varargin{:});
end
