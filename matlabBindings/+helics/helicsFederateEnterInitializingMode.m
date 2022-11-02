function varargout = helicsFederateEnterInitializingMode(varargin)
% Enter the initialization state of a federate.
% 
% @details The initialization state allows initial values to be set and received if the iteration is requested on entry to the execution
% state. This is a blocking call and will block until the core allows it to proceed.
% 
% @param fed The federate to operate on.
% 
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsFederateEnterInitializingMode', varargin{:});
end
