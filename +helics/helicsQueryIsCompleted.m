%{
	Check if an asynchronously executed query has completed.

	@details This function should usually be called after a QueryExecuteAsync function has been called.

	@param query The query object to check if completed.

	@return Will return HELICS_TRUE if an asynchronous query has completed or a regular query call was made with a result,
	and false if an asynchronous query has not completed or is invalid
%}
function varargout = helicsQueryIsCompleted(varargin)
	[varargout{1:nargout}] = helicsMex("helicsQueryIsCompleted", varargin{:});
end
