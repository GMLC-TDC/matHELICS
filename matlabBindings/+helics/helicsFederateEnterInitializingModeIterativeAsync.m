function varargout = helicsFederateEnterInitializingModeIterativeAsync(varargin)
% Non blocking alternative to \ref helicsFederateEnterInitializingModeIterative.
% 
% @details The function helicsFederateEnterInitializationModeIterativeComplete must be called to finish the operation.
% 
% @param fed The federate to operate on.
% 
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsFederateEnterInitializingModeIterativeAsync', varargin{:});
end
