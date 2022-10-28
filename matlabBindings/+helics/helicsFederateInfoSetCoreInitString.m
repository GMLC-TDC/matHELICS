function varargout = helicsFederateInfoSetCoreInitString(varargin)
%  Set the initialization string for the core usually in the form of command line arguments.
% 
%	@param fi The federate info object to alter.
%	@param coreInit A string containing command line arguments to be passed to the core.
% 

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetCoreInitString', varargin{:});
end
