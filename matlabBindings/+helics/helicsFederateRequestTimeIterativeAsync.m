%{
%	Request an iterative time through an asynchronous call.

%	@details This call allows for finer grain control of the iterative process than /ref helicsFederateRequestTime. It takes a time and
%	iteration request, and returns a time and iteration status. Call /ref helicsFederateRequestTimeIterativeComplete to finish the process.

%	@param fed The federate to make the request of.
%	@param requestTime The next desired time.
%	@param iterate The requested iteration mode.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsFederateRequestTimeIterativeAsync(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateRequestTimeIterativeAsync', varargin{:});
end
