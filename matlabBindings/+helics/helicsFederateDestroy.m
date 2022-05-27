%{
	Disconnect and free a federate.
%}
function varargout = helicsFederateDestroy(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateDestroy', varargin{:});
end
