%{
	Execute a query.

	@details The call will block until the query finishes which may require communication or other delays.

	@param query The query object to use in the query.
	@param fed A federate to send the query through.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.


	@return A pointer to a string.  The string will remain valid until the query is freed or executed again.
	The return will be nullptr if fed or query is an invalid object, the return string will be "#invalid" if the query itself was
	invalid.
%}
function varargout = helicsQueryExecute(varargin)
	[varargout{1:nargout}] = helicsMex('helicsQueryExecute', varargin{:});
end
