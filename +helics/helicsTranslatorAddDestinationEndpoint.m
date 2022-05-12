%{
	Add a destination target endpoint to a translator.

	@details The translator will "translate" all message sent to it.  This method adds an endpoint which will receive data published to the
 translator.

	@param trans The given translator.
	@param ept The name of the endpoint the translator sends data to.

	@param[in,out] err A pointer to an error object for catching errors.

%}
function varargout = helicsTranslatorAddDestinationEndpoint(varargin)
	[varargout{1:nargout}] = helicsMex("helicsTranslatorAddDestinationEndpoint", varargin{:});
end
