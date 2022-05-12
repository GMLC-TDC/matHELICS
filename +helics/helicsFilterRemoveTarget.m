%{
	Remove a destination target from a filter.

	@param filt The given filter.
	@param target The named endpoint to remove as a target.


	@param[in,out] err A pointer to an error object for catching errors.

%}
function varargout = helicsFilterRemoveTarget(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFilterRemoveTarget", varargin{:});
end
