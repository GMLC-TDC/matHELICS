function varargout = helicsFederateRequestTimeIterative(varargin)
%	Request an iterative time.

%	@details This call allows for finer grain control of the iterative process than /ref helicsFederateRequestTime. It takes a time and and
%	iteration request, and returns a time and iteration status.

%	@param fed The federate to make the request of.
%	@param requestTime The next desired time.
%	@param iterate The requested iteration mode.

%	@return granted time and HelicsIterationResult.
	[varargout{1:nargout}] = helicsMex('helicsFederateRequestTimeIterative', varargin{:});
end
