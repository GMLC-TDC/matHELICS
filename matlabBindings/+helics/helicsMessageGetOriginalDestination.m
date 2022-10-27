function varargout = helicsMessageGetOriginalDestination(varargin)
 Get the original destination endpoint of a message, the destination may have been modified by filters or other actions.

%	@param message The message object in question.

%	@return A string with the original destination of a message.
	[varargout{1:nargout}] = helicsMex('helicsMessageGetOriginalDestination', varargin{:});
end
