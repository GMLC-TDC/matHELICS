function varargout = helicsFederateEnterInitializingModeIterativeComplete(varargin)
% Complete the call to enter initializing mode Iterative that was initiated with /ref heliceEnterInitializingModeIterativeAsync.  The
% federate will be in created or error mode on return
% 
% @param fed The federate used in the corresponding async call
% 
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsFederateEnterInitializingModeIterativeComplete', varargin{:});
end
