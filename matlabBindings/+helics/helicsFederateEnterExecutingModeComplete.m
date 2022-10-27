function varargout = helicsFederateEnterExecutingModeComplete(varargin)
 Complete the call to /ref helicsFederateEnterExecutingModeAsync.

%	@param fed The federate object to complete the call.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterExecutingModeComplete', varargin{:});
end
