function varargout = helicsQueryExecute(varargin)
%  Execute a query.
% 
%	@details The call will block until the query finishes which may require communication or other delays.
% 
%	@param query The query object to use in the query.
%	@param fed A federate to send the query through.
% 
%
% 
%	@return A pointer to a string.  The string will remain valid until the query is freed or executed again.
%	The return will be nullptr if fed or query is an invalid object, the return string will be "#invalid" if the query itself was
%	invalid.
	[varargout{1:nargout}] = helicsMex('helicsQueryExecute', varargin{:});
end
