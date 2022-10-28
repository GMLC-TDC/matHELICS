function varargout = helicsMessageSetSource(varargin)
%  Set the source of a message.
% 
%	@param message The message object in question.
%	@param src A string containing the source.
% 
	[varargout{1:nargout}] = helicsMex('helicsMessageSetSource', varargin{:});
end
