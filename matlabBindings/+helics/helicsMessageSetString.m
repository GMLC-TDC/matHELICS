function varargout = helicsMessageSetString(varargin)
%  Set the data payload of a message as a string.
% 
%	@param message The message object in question.
%	@param data A null terminated string containing the message data.
% 
%	@param[in,out] err An error object to fill out in case of an error.
	[varargout{1:nargout}] = helicsMex('helicsMessageSetString', varargin{:});
end
