function varargout = helicsQueryFree(varargin)
% Free the memory associated with a query object.

	[varargout{1:nargout}] = helicsMex('helicsQueryFree', varargin{:});
end
