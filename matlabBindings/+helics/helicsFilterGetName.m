function varargout = helicsFilterGetName(varargin)
 Get the name of the filter and store in the given string.

%	@param filt The given filter.

%	@return A string with the name of the filter.
	[varargout{1:nargout}] = helicsMex('helicsFilterGetName', varargin{:});
end
