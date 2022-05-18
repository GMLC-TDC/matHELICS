%{
	Remove a target from a translator.

	@param trans The given translator.
	@param target The name of the interface to remove as a target.


	@param[in,out] err A pointer to an error object for catching errors.

%}
function varargout = helicsTranslatorRemoveTarget(varargin)
	[varargout{1:nargout}] = helicsMex('helicsTranslatorRemoveTarget', varargin{:});
end
