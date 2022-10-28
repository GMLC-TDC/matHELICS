function varargout = helicsBrokerGlobalError(varargin)
%  Generate a global error through a broker. This will terminate the federation.
% 
%	@param broker The broker to generate the global error on.
%	@param errorCode The error code to associate with the global error.
%	@param errorString An error message to associate with the global error.
% 
	[varargout{1:nargout}] = helicsMex('helicsBrokerGlobalError', varargin{:});
end
