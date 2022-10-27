function varargout = helicsQueryCoreExecute(varargin)
 Execute a query directly on a core.

%	@details The call will block until the query finishes which may require communication or other delays.

%	@param query The query object to use in the query.
%	@param core The core to send the query to.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

%	@return A pointer to a string.  The string will remain valid until the query is freed or executed again.
%	The return will be nullptr if core or query is an invalid object, the return string will be "#invalid" if the query itself was
%	invalid.
	[varargout{1:nargout}] = helicsMex('helicsQueryCoreExecute', varargin{:});
end
