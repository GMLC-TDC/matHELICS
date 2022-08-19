%{
%	Set the source of a message.

%	@param message The message object in question.
%	@param src A string containing the source.

%	@param[in,out] err An error object to fill out in case of an error.
%}
function varargout = helicsMessageSetSource(varargin)
	[varargout{1:nargout}] = helicsMex('helicsMessageSetSource', varargin{:});
end
