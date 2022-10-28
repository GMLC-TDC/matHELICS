function varargout = helicsFederateEnterExecutingModeComplete(varargin)
%  Complete the call to /ref helicsFederateEnterExecutingModeAsync.
% 
%	@param fed The federate object to complete the call.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterExecutingModeComplete', varargin{:});
end
