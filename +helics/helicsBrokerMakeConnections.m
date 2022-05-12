%{
	Load a file containing connection information.

	@param broker The broker to generate the connections from.
	@param file A JSON or TOML file containing connection information.

	@param[in,out] err A HelicsError object, can be NULL if the errors are to be ignored.

%}
function varargout = helicsBrokerMakeConnections(varargin)
	[varargout{1:nargout}] = helicsMex("helicsBrokerMakeConnections", varargin{:});
end
