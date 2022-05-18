%{
	Returns the number of pending receives for the specified destination endpoint.

	@param fed The federate to get the number of waiting messages from.
%}
function varargout = helicsFederatePendingMessageCount(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederatePendingMessageCount', varargin{:});
end
