function varargout = helicsMessageSetOriginalSource(varargin)
% Set the original source of a message.
% 
% @param message The message object in question.
% @param src A string containing the new original source.
% 
% @param[in,out] err An error object to fill out in case of an error.

	[varargout{1:nargout}] = helicsMex('helicsMessageSetOriginalSource', varargin{:});
end
