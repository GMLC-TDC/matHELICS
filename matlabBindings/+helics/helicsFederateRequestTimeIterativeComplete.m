function varargout = helicsFederateRequestTimeIterativeComplete(varargin)
%	Complete an iterative time request asynchronous call.

%	@param fed The federate to make the request of.

%	@return tuple of HelicsTime and HelicsIterationResult.
	[varargout{1:nargout}] = helicsMex('helicsFederateRequestTimeIterativeComplete', varargin{:});
end
