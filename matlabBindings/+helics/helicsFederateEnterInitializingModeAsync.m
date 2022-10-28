function varargout = helicsFederateEnterInitializingModeAsync(varargin)
%  Non blocking alternative to \ref helicsFederateEnterInitializingMode.
% 
%	@details The function helicsFederateEnterInitializationModeFinalize must be called to finish the operation.
% 
%	@param fed The federate to operate on.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterInitializingModeAsync', varargin{:});
end
