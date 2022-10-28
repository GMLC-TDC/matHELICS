function varargout = helicsFederateSetTimeRequestEntryCallback(varargin)
%	Set callback for the federate time request.n
%	@details This callback will be executed when a valid time request is made.
%	It is intended for the possibility of embedded data grabbers in a callback to simplify user code.

%	@param fed The federate object in which to set the callback.
%	@param requestTime A callback with the signature void(double currentTime, double requestTime, bool iterating).
	[varargout{1:nargout}] = helicsMex('helicsFederateSetTimeRequestEntryCallback', varargin{:});
end
