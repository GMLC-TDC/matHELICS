function varargout = helicsEndpointSubscribe(varargin)
%  Subscribe an endpoint to a publication.
% 
%	@param endpoint The endpoint to use.
%	@param key The name of the publication.
% 
	[varargout{1:nargout}] = helicsMex('helicsEndpointSubscribe', varargin{:});
end
