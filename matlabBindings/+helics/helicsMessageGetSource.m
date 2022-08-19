%{
%	Get the source endpoint of a message.

%	@param message The message object in question.

%	@return A string with the source endpoint.
%}
function varargout = helicsMessageGetSource(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageGetSource', varargin{:});
end
