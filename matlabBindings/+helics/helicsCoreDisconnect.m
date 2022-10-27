function varargout = helicsCoreDisconnect(varargin)
%  Disconnect a core from the federation.
% 
%	@param core The core to query.
% 

	[varargout{1:nargout}] = helicsMex('helicsCoreDisconnect', varargin{:});
end
