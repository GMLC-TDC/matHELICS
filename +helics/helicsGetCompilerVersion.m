%{
	Get the compiler version used to compile HELICS.
%}
function varargout = helicsGetCompilerVersion(varargin)
	[varargout{1:nargout}] = helicsMex("helicsGetCompilerVersion", varargin{:});
end
