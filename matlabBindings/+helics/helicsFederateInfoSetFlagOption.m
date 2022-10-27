function varargout = helicsFederateInfoSetFlagOption(varargin)
%  Set a flag in the info structure.
% 
%	@details Valid flags are available /ref helics_federate_flags.
%	@param fi The federate info object to alter.
%	@param flag A numerical index for a flag.
%	@param value The desired value of the flag HELICS_TRUE or HELICS_FALSE.
% 
%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetFlagOption', varargin{:});
end
