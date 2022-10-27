function varargout = helicsCoreFree(varargin)
 Release the memory associated with a core.
	[varargout{1:nargout}] = helicsMex('helicsCoreFree', varargin{:});
end
