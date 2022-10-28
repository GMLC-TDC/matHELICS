function varargout = helicsFederateFinalize(varargin)
%  Disconnect/finalize the federate. This function halts all communication in the federate and disconnects it from the core.
	[varargout{1:nargout}] = helicsMex('helicsFederateFinalize', varargin{:});
end
