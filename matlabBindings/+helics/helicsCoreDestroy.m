%{
%	Disconnect and free a core.
%}
function varargout = helicsCoreDestroy(varargin)
	[varargout{1:nargout}] = helicsMex('helicsCoreDestroy', varargin{:});
end
