function varargout = helicsFederateInfoSetCoreInitString(varargin)
%  Set the initialization string for the core usually in the form of command line arguments.
% 
%	@param fi The federate info object to alter.
%	@param coreInit A string containing command line arguments to be passed to the core.
% 
%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetCoreInitString', varargin{:});
end
