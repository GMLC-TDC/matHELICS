%{
%	Get the size of the data payload in bytes.

%	@param message The message object in question.

%	@return The size of the data payload.
%}
function varargout = helicsMessageGetByteCount(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageGetByteCount', varargin{:});
end
