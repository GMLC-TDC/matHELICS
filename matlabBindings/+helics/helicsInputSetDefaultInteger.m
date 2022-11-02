function varargout = helicsInputSetDefaultInteger(varargin)
% Set the default as an integer.
% 
% @param ipt The input to set the default for.
% @param val The default integer.
% 
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultInteger', varargin{:});
end
