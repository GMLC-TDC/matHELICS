%{
%	Get the type of an input.

%	@param ipt The input to query.

%	@return A void enumeration, HELICS_OK if everything worked.
%}
function varargout = helicsInputGetType(varargin)
	[varargout{1:nargout}] = helicsMex('helicsInputGetType', varargin{:});
end
