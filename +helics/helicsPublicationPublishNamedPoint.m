%{
	Publish a named point.

	@param pub The publication to publish for.
	@param str A string for the name to publish.
	@param val A double for the value to publish.

	@param[in,out] err A pointer to an error object for catching errors.

%}
function varargout = helicsPublicationPublishNamedPoint(varargin)
	[varargout{1:nargout}] = helicsMex("helicsPublicationPublishNamedPoint", varargin{:});
end
