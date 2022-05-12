%{
	Disconnect and free a broker.
%}
function varargout = helicsBrokerDestroy(varargin)
	[varargout{1:nargout}] = helicsMex("helicsBrokerDestroy", varargin{:});
end
