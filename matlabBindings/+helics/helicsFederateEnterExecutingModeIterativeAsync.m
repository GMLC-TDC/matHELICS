function varargout = helicsFederateEnterExecutingModeIterativeAsync(varargin)
%  Request an iterative entry to the execution mode.
% 
%	@details This call allows for finer grain control of the iterative process than /ref helicsFederateRequestTime. It takes a time and
%	         iteration request, and returns a time and iteration status
% 
%	@param fed The federate to make the request of.
%	@param iterate The requested iteration mode.
% 
%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterExecutingModeIterativeAsync', varargin{:});
end
