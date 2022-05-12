%{
	Add a destination target to a filter.

	@details All messages going to a destination are copied to the delivery address(es).
	@param filt The given filter to add a destination target to.
	@param dst The name of the endpoint to add as a destination target.

	@param[in,out] err A pointer to an error object for catching errors.

%}
function varargout = helicsFilterAddDestinationTarget(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFilterAddDestinationTarget", varargin{:});
end
