%{
%	Publish a Boolean Value.

%	@param pub The publication to publish for.
%	@param val The boolean value to publish.

%	@param[in,out] err A pointer to an error object for catching errors.

%}
function varargout = helicsPublicationPublishBoolean(varargin)
	[varargout{1:nargout}] = helicsMex('helicsPublicationPublishBoolean', varargin{:});
end
