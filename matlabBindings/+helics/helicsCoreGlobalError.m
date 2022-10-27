function varargout = helicsCoreGlobalError(varargin)
%  Generate a global error through a broker. This will terminate the federation.
% 
%	@param core The core to generate the global error.
%	@param errorCode The error code to associate with the global error.
%	@param errorString An error message to associate with the global error.
% 
%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
	[varargout{1:nargout}] = helicsMex('helicsCoreGlobalError', varargin{:});
end
