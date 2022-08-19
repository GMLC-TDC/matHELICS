%{
%	Execute a query in a non-blocking call.

%	@param query The query object to use in the query.
%	@param fed A federate to send the query through.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsQueryExecuteAsync(varargin)
	[varargout{1:nargout}] = helicsMex('helicsQueryExecuteAsync', varargin{:});
end
