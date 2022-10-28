function varargout = helicsMessageSetOriginalDestination(varargin)
%  Set the original destination of a message.
% 
%	@param message The message object in question.
%	@param dst A string containing the new original source.
% 
	[varargout{1:nargout}] = helicsMex('helicsMessageSetOriginalDestination', varargin{:});
end
