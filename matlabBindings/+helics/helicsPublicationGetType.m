%{
%	Get the type of a publication.

%	@param pub The publication to query.

%	@return A void enumeration, HELICS_OK if everything worked.
%}
function varargout = helicsPublicationGetType(varargin)
	[varargout{1:nargout}] = helicsMex('helicsPublicationGetType', varargin{:});
end
