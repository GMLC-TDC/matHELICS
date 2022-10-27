function varargout = helicsPublicationPublishChar(varargin)
 Publish a single character.

%	@param pub The publication to publish for.
%	@param val The numerical value to publish.

%	@param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsPublicationPublishChar', varargin{:});
end
