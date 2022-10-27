function varargout = helicsBrokerSetGlobal(varargin)
%  Set a federation global value.
% 
%	@details This overwrites any previous value for this name.
% 
%	@param broker The broker to set the global through.
%	@param valueName The name of the global to set.
%	@param value The value of the global.
% 
	[varargout{1:nargout}] = helicsMex('helicsBrokerSetGlobal', varargin{:});
end
