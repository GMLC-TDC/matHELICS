%{
	Free the memory associated with a query object.
%}
function varargout = helicsQueryFree(varargin)
	[varargout{1:nargout}] = helicsMex('helicsQueryFree', varargin{:});
end
