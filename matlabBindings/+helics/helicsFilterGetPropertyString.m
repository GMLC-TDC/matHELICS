function varargout = helicsFilterGetPropertyString(varargin)
% Get a string property on a filter.  The string output memory is valid until a subsequent call to getPropertyString on the particular
% filter.
% 
% @param filt The filter to retrieve a value from.
% @param prop A string containing the property to get.
%

	[varargout{1:nargout}] = helicsMex('helicsFilterGetPropertyString', varargin{:});
end
