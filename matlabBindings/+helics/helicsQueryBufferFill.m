function varargout = helicsQueryBufferFill(varargin)
%	Set the data for a query callback.

%	@details There are many queries that HELICS understands directly, but it is occasionally useful to have a federate be able to respond to specific queries with answers specific to a federate.

%	@param buffer The buffer received in a helicsQueryCallback.
%	@param queryResult The string with the data to fill the buffer with.
	[varargout{1:nargout}] = helicsMex('helicsQueryBufferFill', varargin{:});
end
