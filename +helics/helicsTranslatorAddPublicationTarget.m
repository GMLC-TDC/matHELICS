%{
	Add a source publication target to a translator.

	@details When a publication publishes data the translator will receive it and convert it to a message sent to a translators destination
 endpoints.
	This method adds a publication which publishes data the translator receives and sends to its destination endpoints.

	@param trans The given translator.
	@param pub The name of the publication to subscribe.

	@param[in,out] err A pointer to an error object for catching errors.

%}
function varargout = helicsTranslatorAddPublicationTarget(varargin)
	[varargout{1:nargout}] = helicsMex("helicsTranslatorAddPublicationTarget", varargin{:});
end
