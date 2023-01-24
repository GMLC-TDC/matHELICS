function varargout = helicsInputGetStringSize(varargin)
% Get the size of a value for an input assuming return as a string.
% 
% @return The size of the string.

	[varargout{1:nargout}] = helicsMex('helicsInputGetStringSize', varargin{:});
end
