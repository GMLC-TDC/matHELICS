function varargout = helicsMessageReserve(varargin)
%  Reserve space in a buffer but don't actually resize.
% 
%	@details The message data buffer will be reserved but not resized.
% 
%	@param message The message object in question.
%	@param reserveSize The number of bytes to reserve in the message object.
% 
	[varargout{1:nargout}] = helicsMex('helicsMessageReserve', varargin{:});
end
