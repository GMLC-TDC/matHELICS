function varargout = helicsQueryExecuteComplete(varargin)
%  Complete the return from a query called with /ref helicsExecuteQueryAsync.
% 
%	@details The function will block until the query completes /ref isQueryComplete can be called to determine if a query has completed or
%	not.
% 
%	@param query The query object to complete execution of.
% 
% 
%	@return A pointer to a string. The string will remain valid until the query is freed or executed again.
%	The return will be nullptr if query is an invalid object
	[varargout{1:nargout}] = helicsMex('helicsQueryExecuteComplete', varargin{:});
end
