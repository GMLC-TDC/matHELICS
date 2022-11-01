function varargout = helicsFederateRequestTime(varargin)
% Request the next time for federate execution.
% 
% @param fed The federate to make the request of.
% @param requestTime The next requested time.
% 
% 
% @return The time granted to the federate, will return HELICS_TIME_MAXTIME if the simulation has terminated or is invalid.

	[varargout{1:nargout}] = helicsMex('helicsFederateRequestTime', varargin{:});
end
