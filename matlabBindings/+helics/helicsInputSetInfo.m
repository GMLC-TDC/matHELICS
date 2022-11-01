function varargout = helicsInputSetInfo(varargin)
% Set the data in the info field for an input.
% 
% @param inp The input to query.
% @param info The string to set.
% 
% @param[in,out] err An error object to fill out in case of an error.

	[varargout{1:nargout}] = helicsMex('helicsInputSetInfo', varargin{:});
end
