function varargout = helicsFilterGetPropertyDouble(varargin)
% Get a double property from a filter.
% 
% @param filt The filter to retrieve a value from.
% @param prop A string containing the property to get.
%

	[varargout{1:nargout}] = helicsMex('helicsFilterGetPropertyDouble', varargin{:});
end
