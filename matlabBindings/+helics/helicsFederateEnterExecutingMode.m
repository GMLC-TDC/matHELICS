function varargout = helicsFederateEnterExecutingMode(varargin)
%  Request that the federate enter the Execution mode.
% 
%	@details This call is blocking until granted entry by the core object. On return from this call the federate will be at time 0.
%	         For an asynchronous alternative call see /ref helicsFederateEnterExecutingModeAsync.
% 
%	@param fed A federate to change modes.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterExecutingMode', varargin{:});
end
