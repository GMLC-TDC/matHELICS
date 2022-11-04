function varargout = helicsQueryBrokerExecute(varargin)
% Execute a query directly on a broker.
% 
% @details The call will block until the query finishes which may require communication or other delays.
% 
% @param query The query object to use in the query.
% @param broker The broker to send the query to.
% 
% 
% @return A pointer to a string.  The string will remain valid until the query is freed or executed again.
% The return will be nullptr if broker or query is an invalid object, the return string will be "#invalid" if the query itself was
% invalid

	[varargout{1:nargout}] = helicsMex('helicsQueryBrokerExecute', varargin{:});
end
