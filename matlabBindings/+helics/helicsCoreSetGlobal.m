function varargout = helicsCoreSetGlobal(varargin)
%  Set a global value in a core.
% 
%	@details This overwrites any previous value for this name.
% 
%	@param core The core to set the global through.
%	@param valueName The name of the global to set.
%	@param value The value of the global.
% 
%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
	[varargout{1:nargout}] = helicsMex('helicsCoreSetGlobal', varargin{:});
end
