%{
%	Reserve space in a buffer but don't actually resize.

%	@details The message data buffer will be reserved but not resized.

%	@param message The message object in question.
%	@param reserveSize The number of bytes to reserve in the message object.

%	@param[in,out] err An error object to fill out in case of an error.
%}
function varargout = helicsMessageReserve(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageReserve', varargin{:});
end
