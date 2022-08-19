%{
%	Get a version string for HELICS.
%}
function varargout = helicsGetVersion(varargin)
	[varargout{1:nargout}] = helicsMex('helicsGetVersion', varargin{:});
end
