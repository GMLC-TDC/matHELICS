function varargout = helicsFederateRequestTimeAsync(varargin)
%  Request the next time for federate execution in an asynchronous call.
% 
%	@details Call /ref helicsFederateRequestTimeComplete to finish the call.
% 
%	@param fed The federate to make the request of.
%	@param requestTime The next requested time.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateRequestTimeAsync', varargin{:});
end
