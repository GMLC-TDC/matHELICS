%{
	Set the output delay for a federate.

	@param fi The federate info object to alter.
	@param timeProperty An integer representation of the time based property to set see /ref helics_properties.
	@param propertyValue The value of the property to set the timeProperty to.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsFederateInfoSetTimeProperty(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateInfoSetTimeProperty", varargin{:});
end
