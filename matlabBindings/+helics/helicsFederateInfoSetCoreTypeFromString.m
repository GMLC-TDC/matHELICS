function varargout = helicsFederateInfoSetCoreTypeFromString(varargin)
% Set the core type from a string.
% 
% @param fi The federate info object to alter.
% @param coretype A string naming a core type.
% 
% @param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetCoreTypeFromString', varargin{:});
end
