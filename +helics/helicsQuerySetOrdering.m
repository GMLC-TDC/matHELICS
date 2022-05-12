%{
	Update the ordering mode of the query, fast runs on priority channels, ordered goes on normal channels but goes in sequence

	@param query The query object to change the order for.
	@param mode 0 for fast, 1 for ordered


	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsQuerySetOrdering(varargin)
	[varargout{1:nargout}] = helicsMex("helicsQuerySetOrdering", varargin{:});
end
