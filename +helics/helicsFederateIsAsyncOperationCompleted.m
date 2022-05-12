%{
	Check if the current Asynchronous operation has completed.

	@param fed The federate to operate on.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	@return HELICS_FALSE if not completed, HELICS_TRUE if completed.
%}
function varargout = helicsFederateIsAsyncOperationCompleted(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateIsAsyncOperationCompleted", varargin{:});
end
