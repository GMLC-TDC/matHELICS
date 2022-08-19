%{
%	Get an identifier for the broker.

%	@param broker The broker to query.

%	@return A string containing the identifier for the broker.
%}
function varargout = helicsBrokerGetIdentifier(varargin)
	[varargout{1:nargout}] = helicsMex('helicsBrokerGetIdentifier', varargin{:});
end
