%{
	Request an iterative time.

	@details This call allows for finer grain control of the iterative process than /ref helicsFederateRequestTime. It takes a time and
	         iteration request, and returns a time and iteration status.

	@param fed The federate to make the request of.
	@param iterate The requested iteration mode.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	@return An iteration structure with field containing the time and iteration status.
%}
function varargout = helicsFederateEnterExecutingModeIterative(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterExecutingModeIterative', varargin{:});
end
