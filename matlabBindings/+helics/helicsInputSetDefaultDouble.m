function varargout = helicsInputSetDefaultDouble(varargin)
%  Set the default as a double.
% 
%	@param ipt The input to set the default for.
%	@param val The default double value.
% 
%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultDouble', varargin{:});
end
