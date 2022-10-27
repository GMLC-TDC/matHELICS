function varargout = helicsFederateInfoSetCoreType(varargin)
%  Set the core type by integer code.
% 
%	@details Valid values available by definitions in api-data.h.
%	@param fi The federate info object to alter.
%	@param coretype An numerical code for a core type see /ref helics_CoreType.
% 

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetCoreType', varargin{:});
end
