function varargout = helicsCoreMakeConnections(varargin)
%  Load a file containing connection information.
% 
%	@param core The core to generate the connections from.
%	@param file A JSON or TOML file containing connection information.
% 

	[varargout{1:nargout}] = helicsMex('helicsCoreMakeConnections', varargin{:});
end
