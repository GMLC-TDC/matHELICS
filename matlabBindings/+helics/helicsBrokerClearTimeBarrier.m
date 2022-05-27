%{
	Clear any time barrier on a broker.

	@param broker The broker to clear the barriers on.
%}
function varargout = helicsBrokerClearTimeBarrier(varargin)
	[varargout{1:nargout}] = helicsMex('helicsBrokerClearTimeBarrier', varargin{:});
end
