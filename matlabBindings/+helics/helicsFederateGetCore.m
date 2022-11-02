function varargout = helicsFederateGetCore(varargin)
% Get the core object associated with a federate.
% 
% @param fed A federate object.
% 
% 
% @return A core object, nullptr if invalid.

	[varargout{1:nargout}] = helicsMex('helicsFederateGetCore', varargin{:});
end
