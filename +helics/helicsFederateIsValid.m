%{
	Check if a federate_object is valid.

	@return HELICS_TRUE if the federate is a valid active federate, HELICS_FALSE otherwise
%}
function varargout = helicsFederateIsValid(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateIsValid", varargin{:});
end
