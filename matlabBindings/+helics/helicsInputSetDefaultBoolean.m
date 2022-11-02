function varargout = helicsInputSetDefaultBoolean(varargin)
% Set the default as a boolean.
% 
% @param ipt The input to set the default for.
% @param val The default boolean value.
% 
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultBoolean', varargin{:});
end
