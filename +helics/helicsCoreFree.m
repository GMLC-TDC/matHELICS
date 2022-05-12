%{
	Release the memory associated with a core.
%}
function varargout = helicsCoreFree(varargin)
	[varargout{1:nargout}] = helicsMex("helicsCoreFree", varargin{:});
end
