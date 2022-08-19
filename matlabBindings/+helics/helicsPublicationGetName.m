%{
%	Get the name of a publication.

%	@details This will be the global key used to identify the publication to the federation.

%	@param pub The publication to query.

%	@return A const char with the publication name.
%}
function varargout = helicsPublicationGetName(varargin)
	[varargout{1:nargout}] = helicsMex('helicsPublicationGetName', varargin{:});
end
