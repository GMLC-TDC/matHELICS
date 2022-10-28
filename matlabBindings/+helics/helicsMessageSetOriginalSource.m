function varargout = helicsMessageSetOriginalSource(varargin)
%  Set the original source of a message.
% 
%	@param message The message object in question.
%	@param src A string containing the new original source.
% 
	[varargout{1:nargout}] = helicsMex('helicsMessageSetOriginalSource', varargin{:});
end
