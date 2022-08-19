%{
%	Receive a packet from a particular endpoint.

%	@param[in] endpoint The identifier for the endpoint.

%	@return A message object.
%}
function varargout = helicsEndpointGetMessage(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointGetMessage', varargin{:});
end
