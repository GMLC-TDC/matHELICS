%{
%	Get the destination endpoint of a message.

%	@param message The message object in question.

%	@return A string with the destination endpoint.
%}
function varargout = helicsMessageGetDestination(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageGetDestination', varargin{:});
end
