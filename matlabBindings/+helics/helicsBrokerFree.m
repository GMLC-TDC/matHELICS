%{
%	Release the memory associated with a broker.
%}
function varargout = helicsBrokerFree(varargin)
	[varargout{1:nargout}] = helicsMex('helicsBrokerFree', varargin{:});
end
