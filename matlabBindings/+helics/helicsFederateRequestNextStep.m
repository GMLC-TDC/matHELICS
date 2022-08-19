%{
%	Request the next time step for federate execution.

%	@details Feds should have setup the period or minDelta for this to work well but it will request the next time step which is the current
%	time plus the minimum time step.

%	@param fed The federate to make the request of.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

%	@return The time granted to the federate, will return HELICS_TIME_MAXTIME if the simulation has terminated or is invalid
%}
function varargout = helicsFederateRequestNextStep(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateRequestNextStep', varargin{:});
end
