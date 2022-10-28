function varargout = helicsBrokerDataLink(varargin)
%  Link a named publication and named input using a broker.
% 
%	@param broker The broker to generate the connection from.
%	@param source The name of the publication (cannot be NULL).
%	@param target The name of the target to send the publication data (cannot be NULL).
% 

	[varargout{1:nargout}] = helicsMex('helicsBrokerDataLink', varargin{:});
end
