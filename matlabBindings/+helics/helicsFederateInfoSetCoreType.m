%{
	Set the core type by integer code.

	@details Valid values available by definitions in api-data.h.
	@param fi The federate info object to alter.
	@param coretype An numerical code for a core type see /ref helics_CoreType.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

%}
function varargout = helicsFederateInfoSetCoreType(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetCoreType', varargin{:});
end
