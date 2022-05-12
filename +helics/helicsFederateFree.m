%{
	Release the memory associated with a federate.
%}
function varargout = helicsFederateFree(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateFree", varargin{:});
end
