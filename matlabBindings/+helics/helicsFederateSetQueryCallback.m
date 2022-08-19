%{
%	Set callback for queries executed against a federate.

%	@details There are many queries that HELICS understands directly, but it is occasionally useful to a have a federate be able to respond
%	tospecific queries with answers specific to a federate.

%	@param fed The federate object in which to set the callback.
%	@param queryAnswer A function handle with the signature const void(const char *query, int querySize, HelicsQueryBuffer buffer).
%}
function varargout = helicsFederateSetQueryCallback(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateSetQueryCallback', varargin{:});
end
