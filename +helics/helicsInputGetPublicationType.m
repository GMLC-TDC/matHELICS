%{
	Get the type the publisher to an input is sending.

	@param ipt The input to query.

	@return A const char with the type name.
%}
function varargout = helicsInputGetPublicationType(varargin)
	[varargout{1:nargout}] = helicsMex("helicsInputGetPublicationType", varargin{:});
end
