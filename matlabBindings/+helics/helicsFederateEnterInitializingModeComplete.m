function varargout = helicsFederateEnterInitializingModeComplete(varargin)
%  Finalize the entry to initialize mode that was initiated with /ref heliceEnterInitializingModeAsync.
% 
%	@param fed The federate desiring to complete the initialization step.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterInitializingModeComplete', varargin{:});
end
