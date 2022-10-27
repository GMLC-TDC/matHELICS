function varargout = helicsMessageIsValid(varargin)
 A check if the message contains a valid payload.

%	@param message The message object in question.

%	@return HELICS_TRUE if the message contains a payload.
	[varargout{1:nargout}] = helicsMex('helicsMessageIsValid', varargin{:});
end
