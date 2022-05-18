%{
	Disconnect/finalize the federate. This function halts all communication in the federate and disconnects it from the core.
%}
function varargout = helicsFederateFinalize(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateFinalize', varargin{:});
end
