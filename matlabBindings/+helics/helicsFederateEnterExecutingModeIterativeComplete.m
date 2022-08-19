%{
%	Complete the asynchronous iterative call into ExecutionMode.

%	@param fed The federate to make the request of.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

%	@return An iteration object containing the iteration time and iteration_status.
%}
function varargout = helicsFederateEnterExecutingModeIterativeComplete(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterExecutingModeIterativeComplete', varargin{:});
end
