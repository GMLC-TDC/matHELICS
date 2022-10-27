function varargout = helicsFilterRemoveTarget(varargin)
%  Remove a destination target from a filter.
% 
%	@param filt The given filter.
%	@param target The named endpoint to remove as a target.
% 
% 

	[varargout{1:nargout}] = helicsMex('helicsFilterRemoveTarget', varargin{:});
end
