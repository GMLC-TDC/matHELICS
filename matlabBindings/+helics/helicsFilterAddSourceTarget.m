function varargout = helicsFilterAddSourceTarget(varargin)
% Add a source target to a filter.
% 
% @details All messages coming from a source are copied to the delivery address(es).
% 
% @param filt The given filter.
% @param source The name of the endpoint to add as a source target.
% 
% @param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsFilterAddSourceTarget', varargin{:});
end
