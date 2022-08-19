%{
%	Check if the federate has any outstanding messages.

%	@param fed The federate to check.

%	@return HELICS_TRUE if the federate has a message waiting, HELICS_FALSE otherwise.
%}
function varargout = helicsFederateHasMessage(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateHasMessage', varargin{:});
end
