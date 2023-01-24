function varargout = helicsInputGetVectorSize(varargin)
% Get the size of a value for an ionput assuming return as an array of doubles.
% 
% @return The number of doubles in a returned vector.

	[varargout{1:nargout}] = helicsMex('helicsInputGetVectorSize', varargin{:});
end
