%{
	Finalize the entry to initialize mode that was initiated with /ref heliceEnterInitializingModeAsync.

	@param fed The federate desiring to complete the initialization step.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsFederateEnterInitializingModeComplete(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterInitializingModeComplete', varargin{:});
end
