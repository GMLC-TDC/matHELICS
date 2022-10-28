function varargout = helicsFederateEnterExecutingModeAsync(varargin)
%  Request that the federate enter the Execution mode.
% 
%	@details This call is non-blocking and will return immediately. Call /ref helicsFederateEnterExecutingModeComplete to finish the call
%	sequence.
% 
%	@param fed The federate object to complete the call.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterExecutingModeAsync', varargin{:});
end
