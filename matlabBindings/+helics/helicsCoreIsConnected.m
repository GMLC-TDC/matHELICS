function varargout = helicsCoreIsConnected(varargin)
%  Check if a core is connected.
% 
%	@details A connected core implies it is attached to federates or federates could be attached to it
% 
%	@return HELICS_FALSE if not connected, HELICS_TRUE if it is connected.
	[varargout{1:nargout}] = helicsMex('helicsCoreIsConnected', varargin{:});
end
