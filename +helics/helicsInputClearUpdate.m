%{
	Clear the updated flag from an input.
%}
function varargout = helicsInputClearUpdate(varargin)
	[varargout{1:nargout}] = helicsMex('helicsInputClearUpdate', varargin{:});
end
