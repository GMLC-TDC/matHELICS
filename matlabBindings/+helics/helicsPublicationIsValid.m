function varargout = helicsPublicationIsValid(varargin)
% Check if a publication is valid.
% 
% @param pub The publication to check.
% 
% @return HELICS_TRUE if the publication is a valid publication.

	[varargout{1:nargout}] = helicsMex('helicsPublicationIsValid', varargin{:});
end
