function varargout = helicsInputGetTarget(varargin)
% Get the target of an input.
% 
% @return A const char with the input target.

	[varargout{1:nargout}] = helicsMex('helicsInputGetTarget', varargin{:});
end
