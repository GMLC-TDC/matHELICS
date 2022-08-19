%{
%	A check if the message contains a valid payload.

%	@param message The message object in question.

%	@return HELICS_TRUE if the message contains a payload.
%}
function varargout = helicsMessageIsValid(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageIsValid', varargin{:});
end
