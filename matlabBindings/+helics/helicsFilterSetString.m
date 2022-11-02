function varargout = helicsFilterSetString(varargin)
% Set a string property on a filter.
% 
% @param filt The filter to modify.
% @param prop A string containing the property to set.
% @param val A string containing the new value.
% 
% @param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsFilterSetString', varargin{:});
end
