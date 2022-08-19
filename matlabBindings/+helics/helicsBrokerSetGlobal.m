%{
%	Set a federation global value.

%	@details This overwrites any previous value for this name.

%	@param broker The broker to set the global through.
%	@param valueName The name of the global to set.
%	@param value The value of the global.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsBrokerSetGlobal(varargin)
	[varargout{1:nargout}] = helicsMex('helicsBrokerSetGlobal', varargin{:});
end
