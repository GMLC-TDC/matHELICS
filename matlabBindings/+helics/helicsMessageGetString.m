function varargout = helicsMessageGetString(varargin)
 Get the payload of a message as a string.

%	@param message The message object in question.

%	@return A string representing the payload of a message.
	[varargout{1:nargout}] = helicsMex('helicsMessageGetString', varargin{:});
end
