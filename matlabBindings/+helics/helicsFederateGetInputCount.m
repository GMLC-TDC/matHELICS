%{
	Get the number of subscriptions in a federate.

	@return (-1) if fed was not a valid federate otherwise returns the number of subscriptions.
%}
function varargout = helicsFederateGetInputCount(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateGetInputCount', varargin{:});
end
