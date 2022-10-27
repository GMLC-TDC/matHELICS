function varargout = helicsMessageGetSource(varargin)
 Get the source endpoint of a message.

%	@param message The message object in question.

%	@return A string with the source endpoint.
	[varargout{1:nargout}] = helicsMex('helicsMessageGetSource', varargin{:});
end
