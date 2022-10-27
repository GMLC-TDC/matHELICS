function varargout = helicsFederateInfoSetCoreName(varargin)
%  Set the name of the core to link to for a federate.
% 
%	@param fi The federate info object to alter.
%	@param corename The identifier for a core to link to.
% 

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetCoreName', varargin{:});
end
