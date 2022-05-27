%{
	Set a flag for the federate.

	@param fed The federate to alter a flag for.
	@param flag The flag to change.
	@param flagValue The new value of the flag. 0 for false, !=0 for true.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsFederateSetFlagOption(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateSetFlagOption', varargin{:});
end
