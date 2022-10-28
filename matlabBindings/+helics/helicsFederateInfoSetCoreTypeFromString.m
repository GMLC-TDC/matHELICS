function varargout = helicsFederateInfoSetCoreTypeFromString(varargin)
%  Set the core type from a string.
% 
%	@param fi The federate info object to alter.
%	@param coretype A string naming a core type.
% 

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetCoreTypeFromString', varargin{:});
end
