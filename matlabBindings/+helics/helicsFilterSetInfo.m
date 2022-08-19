%{
%	Set the data in the info field for a filter.

%	@param filt The given filter.
%	@param info The string to set.

%	@param[in,out] err An error object to fill out in case of an error.

%}
function varargout = helicsFilterSetInfo(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFilterSetInfo', varargin{:});
end
