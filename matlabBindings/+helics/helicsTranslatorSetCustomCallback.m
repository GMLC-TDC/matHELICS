%{
	Set a general callback for a custom Translator.n
	@details Add a pair of custom callbacks for running a translator operation in the C shared library.
	@param translator The translator object to set the callbacks for.
	@param toMessageCall A callback with the signature void(HelicsDataBuffer, HelicsMessage).
	@param toValueCall A callback with the signature void(HelicsMessage, HelicsDataBuffer).
%}
function varargout = helicsTranslatorSetCustomCallback(varargin)
	[varargout{1:nargout}] = helicsMex('helicsTranslatorSetCustomCallback', varargin{:});
end
