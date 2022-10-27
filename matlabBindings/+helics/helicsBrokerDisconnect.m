function varargout = helicsBrokerDisconnect(varargin)
%  Disconnect a broker.
% 
%	@param broker The broker to disconnect.
% 
%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsBrokerDisconnect', varargin{:});
end
