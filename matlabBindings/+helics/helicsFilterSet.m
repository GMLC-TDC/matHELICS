function varargout = helicsFilterSet(varargin)
%  Set a property on a filter.
% 
%	@param filt The filter to modify.
%	@param prop A string containing the property to set.
%	@param val A numerical value for the property.
% 

	[varargout{1:nargout}] = helicsMex('helicsFilterSet', varargin{:});
end
