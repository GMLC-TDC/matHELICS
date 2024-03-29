function varargout = helicsFederateRequestTimeComplete(varargin)
% Complete an asynchronous requestTime call.
% 
% @param fed The federate to make the request of.
% 
% 
% @return The time granted to the federate, will return HELICS_TIME_MAXTIME if the simulation has terminated.

	[varargout{1:nargout}] = helicsMex('helicsFederateRequestTimeComplete', varargin{:});
end
