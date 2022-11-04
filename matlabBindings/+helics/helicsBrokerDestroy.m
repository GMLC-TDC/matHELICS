function varargout = helicsBrokerDestroy(varargin)
% Disconnect and free a broker.

	[varargout{1:nargout}] = helicsMex('helicsBrokerDestroy', varargin{:});
end
