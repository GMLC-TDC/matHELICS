%{
	Check if a broker is connected.

	@details A connected broker implies it is attached to cores or cores could reach out to communicate.

	@return HELICS_FALSE if not connected.
%}
function varargout = helicsBrokerIsConnected(varargin)
	[varargout{1:nargout}] = helicsMex('helicsBrokerIsConnected', varargin{:});
end
