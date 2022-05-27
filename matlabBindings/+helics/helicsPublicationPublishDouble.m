%{
	Publish a double floating point value.

	@param pub The publication to publish for.
	@param val The numerical value to publish.

	@param[in,out] err A pointer to an error object for catching errors.

%}
function varargout = helicsPublicationPublishDouble(varargin)
	[varargout{1:nargout}] = helicsMex('helicsPublicationPublishDouble', varargin{:});
end
