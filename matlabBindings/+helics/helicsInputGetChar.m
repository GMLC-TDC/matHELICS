function varargout = helicsInputGetChar(varargin)
% Get a single character value from an input.
% 
% @param ipt The input to get the data for.
% 
% 
% @return The resulting character value.
%         NAK (negative acknowledgment) symbol returned on error

	[varargout{1:nargout}] = helicsMex('helicsInputGetChar', varargin{:});
end
