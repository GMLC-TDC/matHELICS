%{
	Non blocking alternative to \ref helicsFederateEnterInitializingMode.

	@details The function helicsFederateEnterInitializationModeFinalize must be called to finish the operation.

	@param fed The federate to operate on.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsFederateEnterInitializingModeAsync(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateEnterInitializingModeAsync", varargin{:});
end
