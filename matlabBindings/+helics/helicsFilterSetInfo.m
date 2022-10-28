function varargout = helicsFilterSetInfo(varargin)
%  Set the data in the info field for a filter.
% 
%	@param filt The given filter.
%	@param info The string to set.
% 

	[varargout{1:nargout}] = helicsMex('helicsFilterSetInfo', varargin{:});
end
