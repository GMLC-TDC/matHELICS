function varargout = helicsMessageSetString(varargin)
%  Set the data payload of a message as a string.
% 
%	@param message The message object in question.
%	@param data A null terminated string containing the message data.
% 
	[varargout{1:nargout}] = helicsMex('helicsMessageSetString', varargin{:});
end
