%{
%	Complete an asynchronous requestTime call.

%	@param fed The federate to make the request of.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

%	@return The time granted to the federate, will return HELICS_TIME_MAXTIME if the simulation has terminated.
%}
function varargout = helicsFederateRequestTimeComplete(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateRequestTimeComplete', varargin{:});
end
