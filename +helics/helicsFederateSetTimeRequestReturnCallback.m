%{
	Set callback for the federate time request return.n
	@details This callback will be executed when after all other callbacks for a time request return.
	This callback will be the last thing executed before returning control the user program.

	@param fed The federate object in which to set the callback.
	@param requestTimeReturn A callback with the signature void(double newTime, bool iterating).
%}
function varargout = helicsFederateSetTimeRequestReturnCallback(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateSetTimeRequestReturnCallback", varargin{:});
end
