function varargout = helicsFederateRequestTimeAsync(varargin)
%  Request the next time for federate execution in an asynchronous call.
% 
%	@details Call /ref helicsFederateRequestTimeComplete to finish the call.
% 
%	@param fed The federate to make the request of.
%	@param requestTime The next requested time.
% 
%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
	[varargout{1:nargout}] = helicsMex('helicsFederateRequestTimeAsync', varargin{:});
end
