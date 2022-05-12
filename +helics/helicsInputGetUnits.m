%{
	Get the units of an input.

	@param ipt The input to query.

	@return A void enumeration, HELICS_OK if everything worked.
%}
function varargout = helicsInputGetUnits(varargin)
	[varargout{1:nargout}] = helicsMex("helicsInputGetUnits", varargin{:});
end
