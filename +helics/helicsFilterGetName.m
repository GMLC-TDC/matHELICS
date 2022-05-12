%{
	Get the name of the filter and store in the given string.

	@param filt The given filter.

	@return A string with the name of the filter.
%}
function varargout = helicsFilterGetName(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFilterGetName", varargin{:});
end
