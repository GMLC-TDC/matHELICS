%{
	Get the build flags used to compile HELICS.
%}
function varargout = helicsGetBuildFlags(varargin)
	[varargout{1:nargout}] = helicsMex("helicsGetBuildFlags", varargin{:});
end
