%{
%	Get the current state of a federate.

%	@param fed The federate to query.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

%	@return State the resulting state if void return HELICS_OK.
%}
function varargout = helicsFederateGetState(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateGetState', varargin{:});
end
