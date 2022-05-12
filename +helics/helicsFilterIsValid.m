%{
	Check if a filter is valid.

	@param filt The filter object to check.

	@return HELICS_TRUE if the Filter object represents a valid filter.
%}
function varargout = helicsFilterIsValid(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFilterIsValid", varargin{:});
end
