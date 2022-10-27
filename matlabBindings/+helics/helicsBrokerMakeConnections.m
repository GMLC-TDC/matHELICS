function varargout = helicsBrokerMakeConnections(varargin)
%  Load a file containing connection information.
% 
%	@param broker The broker to generate the connections from.
%	@param file A JSON or TOML file containing connection information.
% 

	[varargout{1:nargout}] = helicsMex('helicsBrokerMakeConnections', varargin{:});
end
