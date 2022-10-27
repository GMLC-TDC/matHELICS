function varargout = helicsInputSetDefaultString(varargin)
%  Set the default as a string.
% 
%	@param ipt The input to set the default for.
%	@param defaultString A pointer to the default string.
% 
%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultString', varargin{:});
end
