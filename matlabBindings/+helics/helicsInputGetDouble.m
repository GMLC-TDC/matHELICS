function varargout = helicsInputGetDouble(varargin)
% Get a double value from an input.
% 
% @param ipt The input to get the data for.
% 
% 
% @return The double value of the input.

	[varargout{1:nargout}] = helicsMex('helicsInputGetDouble', varargin{:});
end
