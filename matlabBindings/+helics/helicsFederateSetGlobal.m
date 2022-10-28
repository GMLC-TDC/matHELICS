function varargout = helicsFederateSetGlobal(varargin)
%  Set a federation global value through a federate.
% 
%	@details This overwrites any previous value for this name.
%	@param fed The federate to set the global through.
%	@param valueName The name of the global to set.
%	@param value The value of the global.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateSetGlobal', varargin{:});
end
