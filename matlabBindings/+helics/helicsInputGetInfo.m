function varargout = helicsInputGetInfo(varargin)
% Get the data in the info field of an input.
% 
% @param inp The input to query.
% 
% @return A string with the info field string.

	[varargout{1:nargout}] = helicsMex('helicsInputGetInfo', varargin{:});
end
