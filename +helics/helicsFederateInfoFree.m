%{
	Delete the memory associated with a federate info object.
%}
function varargout = helicsFederateInfoFree(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateInfoFree", varargin{:});
end
