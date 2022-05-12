%{
	Set an integer based property of a federate.

	@param fed The federate to change the property for.
	@param intProperty The property to set.
	@param propertyVal The value of the property.

	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsFederateSetIntegerProperty(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateSetIntegerProperty", varargin{:});
end
