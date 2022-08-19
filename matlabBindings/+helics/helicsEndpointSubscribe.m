%{
%	Subscribe an endpoint to a publication.

%	@param endpoint The endpoint to use.
%	@param key The name of the publication.

%	@param[in,out] err A pointer to an error object for catching errors.
%}
function varargout = helicsEndpointSubscribe(varargin)
	[varargout{1:nargout}] = helicsMex('helicsEndpointSubscribe', varargin{:});
end
