function varargout = helicsFederateInfoSetFlagOption(varargin)
%  Set a flag in the info structure.
% 
%	@details Valid flags are available /ref helics_federate_flags.
%	@param fi The federate info object to alter.
%	@param flag A numerical index for a flag.
%	@param value The desired value of the flag HELICS_TRUE or HELICS_FALSE.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetFlagOption', varargin{:});
end
