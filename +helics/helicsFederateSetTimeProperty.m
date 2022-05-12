%{
	Set a time based property for a federate.

	@param fed The federate object to set the property for.
	@param timeProperty A integer code for a time property.
	@param time The requested value of the property.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsFederateSetTimeProperty(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateSetTimeProperty", varargin{:});
end
