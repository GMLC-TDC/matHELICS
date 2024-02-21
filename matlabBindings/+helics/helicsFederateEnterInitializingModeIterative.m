function varargout = helicsFederateEnterInitializingModeIterative(varargin)
% Trigger a blocking call and return to created state after all federates have either triggered an iteration or are waiting to enter
% initializing mode.
% 
% @details This call will return the federate to the created state to allow additional setup to occur with federates either iterating in
% the mode or waiting.
% 
% @param fed The federate to operate on.
% 
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsFederateEnterInitializingModeIterative', varargin{:});
end
