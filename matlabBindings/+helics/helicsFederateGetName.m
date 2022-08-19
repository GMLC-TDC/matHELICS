%{
%	Get the name of the federate.

%	@param fed The federate object to query.

%	@return A pointer to a string with the name.
%}
function varargout = helicsFederateGetName(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateGetName', varargin{:});
end
