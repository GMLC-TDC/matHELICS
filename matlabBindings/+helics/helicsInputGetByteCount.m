function varargout = helicsInputGetByteCount(varargin)
% Get the size of the raw value for subscription.
% 
% @return The size of the raw data/string in bytes.

	[varargout{1:nargout}] = helicsMex('helicsInputGetByteCount', varargin{:});
end
