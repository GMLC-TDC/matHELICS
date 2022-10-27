function varargout = helicsMessageResize(varargin)
%  Resize the data buffer for a message.
% 
%	@details The message data buffer will be resized. There are no guarantees on what is in the buffer in newly allocated space.
%	         If the allocated space is not sufficient new allocations will occur.
% 
%	@param message The message object in question.
%	@param newSize The new size in bytes of the buffer.
% 
	[varargout{1:nargout}] = helicsMex('helicsMessageResize', varargin{:});
end
