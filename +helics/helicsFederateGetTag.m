%{
	Get a federate tag value.

	@param fed The federate to get the tag for.
	@param tagName The name of the tag to query.

	@param[in,out] err A pointer to an error object for catching errors.
%}
function varargout = helicsFederateGetTag(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateGetTag", varargin{:});
end
