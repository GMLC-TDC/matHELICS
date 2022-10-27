function varargout = helicsFilterGetInfo(varargin)
 Get the data in the info field of a filter.

%	@param filt The given filter.

%	@return A string with the info field string.
	[varargout{1:nargout}] = helicsMex('helicsFilterGetInfo', varargin{:});
end
