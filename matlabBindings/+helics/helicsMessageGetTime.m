function varargout = helicsMessageGetTime(varargin)
%  Get the helics time associated with a message.
% 
%	@param message The message object in question.
% 
%	@return The time associated with a message.
	[varargout{1:nargout}] = helicsMex('helicsMessageGetTime', varargin{:});
end
