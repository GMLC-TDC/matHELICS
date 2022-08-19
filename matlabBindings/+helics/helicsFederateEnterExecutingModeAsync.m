%{
%	Request that the federate enter the Execution mode.

%	@details This call is non-blocking and will return immediately. Call /ref helicsFederateEnterExecutingModeComplete to finish the call
%	sequence.

%	@param fed The federate object to complete the call.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsFederateEnterExecutingModeAsync(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterExecutingModeAsync', varargin{:});
end
