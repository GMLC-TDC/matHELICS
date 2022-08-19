%{
%	Disconnect a core from the federation.

%	@param core The core to query.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

%}
function varargout = helicsCoreDisconnect(varargin)
	[varargout{1:nargout}] = helicsMex('helicsCoreDisconnect', varargin{:});
end
