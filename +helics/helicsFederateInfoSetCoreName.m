%{
	Set the name of the core to link to for a federate.

	@param fi The federate info object to alter.
	@param corename The identifier for a core to link to.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

%}
function varargout = helicsFederateInfoSetCoreName(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetCoreName', varargin{:});
end
