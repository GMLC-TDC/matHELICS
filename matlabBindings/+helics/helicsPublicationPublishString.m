function varargout = helicsPublicationPublishString(varargin)
% Publish a string.
% 
% @param pub The publication to publish for.
% @param val The null terminated string to publish.
% 
% @param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsPublicationPublishString', varargin{:});
end
