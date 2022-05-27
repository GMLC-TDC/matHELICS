%{
	Add a named input to the list of targets a publication publishes to.

	@param pub The publication to add the target for.
	@param target The name of an input that the data should be sent to.

	@param[in,out] err A pointer to an error object for catching errors.

%}
function varargout = helicsPublicationAddTarget(varargin)
	[varargout{1:nargout}] = helicsMex('helicsPublicationAddTarget', varargin{:});
end
