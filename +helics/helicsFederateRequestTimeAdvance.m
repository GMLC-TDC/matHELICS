%{
	Request the next time for federate execution.

	@param fed The federate to make the request of.
	@param timeDelta The requested amount of time to advance.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	@return The time granted to the federate, will return HELICS_TIME_MAXTIME if the simulation has terminated or is invalid
%}
function varargout = helicsFederateRequestTimeAdvance(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateRequestTimeAdvance", varargin{:});
end
