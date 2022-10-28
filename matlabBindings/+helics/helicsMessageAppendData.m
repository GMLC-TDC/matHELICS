function varargout = helicsMessageAppendData(varargin)
%	Append data to the payload.

%	@param message The message object in question.
%	@param data A string containing the message data to append.
	[varargout{1:nargout}] = helicsMex('helicsMessageAppendData', varargin{:});
end
