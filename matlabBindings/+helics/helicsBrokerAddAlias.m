function varargout = helicsBrokerAddAlias(varargin)
% Create an alias for an interface.
% 
% @param broker The broker to use to set the alias.
% @param interfaceName The current name of an interface.
% @param alias The additional name to use for the given interface.
% 
% @param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsBrokerAddAlias', varargin{:});
end
