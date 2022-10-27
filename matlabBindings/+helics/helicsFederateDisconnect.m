function varargout = helicsFederateDisconnect(varargin)
 Disconnect/finalize the federate. This function halts all communication in the federate and disconnects it
%	from the core.  This call is identical to helicsFederateFinalize.
	[varargout{1:nargout}] = helicsMex('helicsFederateDisconnect', varargin{:});
end
