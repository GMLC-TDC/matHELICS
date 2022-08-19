%{
%	Get the network address associated with a core.

%	@param core The core to query.

%	@return A string with the network address of the broker.
%}
function varargout = helicsCoreGetAddress(varargin)
	[varargout{1:nargout}] = helicsMex('helicsCoreGetAddress', varargin{:});
end
