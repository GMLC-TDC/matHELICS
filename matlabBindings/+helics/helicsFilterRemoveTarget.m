function varargout = helicsFilterRemoveTarget(varargin)
%  Remove a destination target from a filter.
% 
%	@param filt The given filter.
%	@param target The named endpoint to remove as a target.
% 
% 
%	@param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsFilterRemoveTarget', varargin{:});
end
