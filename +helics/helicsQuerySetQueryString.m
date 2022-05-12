%{
	Update the queryString of a query.

	@param query The query object to change the target of.
	@param queryString the new queryString

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsQuerySetQueryString(varargin)
	[varargout{1:nargout}] = helicsMex("helicsQuerySetQueryString", varargin{:});
end
