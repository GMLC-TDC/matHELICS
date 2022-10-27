function varargout = helicsInputSetDefaultTime(varargin)
 Set the default as a time.

%	@param ipt The input to set the default for.
%	@param val The default time value.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultTime', varargin{:});
end
