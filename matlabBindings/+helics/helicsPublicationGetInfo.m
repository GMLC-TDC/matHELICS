%{
%	Get the data in the info field of an publication.

%	@param pub The publication to query.

%	@return A string with the info field string.
%}
function varargout = helicsPublicationGetInfo(varargin)
	[varargout{1:nargout}] = helicsMex('helicsPublicationGetInfo', varargin{:});
end
