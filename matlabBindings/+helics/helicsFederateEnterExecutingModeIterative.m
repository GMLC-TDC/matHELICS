function varargout = helicsFederateEnterExecutingModeIterative(varargin)
%  Request an iterative time.
% 
%	@details This call allows for finer grain control of the iterative process than /ref helicsFederateRequestTime. It takes a time and
%	         iteration request, and returns a time and iteration status.
% 
%	@param fed The federate to make the request of.
%	@param iterate The requested iteration mode.
% 
% 
%	@return An iteration structure with field containing the time and iteration status.
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterExecutingModeIterative', varargin{:});
end
