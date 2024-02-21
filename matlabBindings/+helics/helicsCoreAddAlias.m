function varargout = helicsCoreAddAlias(varargin)
% Create an alias for an interface.
% 
% @param core The core to use to set the alias.
% @param interfaceName The current name of an interface.
% @param alias The additional name to use for the given interface.
% 
% @param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsCoreAddAlias', varargin{:});
end
