function varargout = helicsMessageSetDestination(varargin)
 Set the destination of a message.

%	@param message The message object in question.
%	@param dst A string containing the new destination.

%	@param[in,out] err An error object to fill out in case of an error.
	[varargout{1:nargout}] = helicsMex('helicsMessageSetDestination', varargin{:});
end
