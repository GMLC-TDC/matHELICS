%{
%	Get the current value of a time based property in a federate.

%	@param fed The federate query.
%	@param timeProperty The property to query.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsFederateGetTimeProperty(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateGetTimeProperty', varargin{:});
end
