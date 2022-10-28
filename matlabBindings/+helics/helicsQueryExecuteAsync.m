function varargout = helicsQueryExecuteAsync(varargin)
%  Execute a query in a non-blocking call.
% 
%	@param query The query object to use in the query.
%	@param fed A federate to send the query through.
% 
	[varargout{1:nargout}] = helicsMex('helicsQueryExecuteAsync', varargin{:});
end
