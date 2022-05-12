%{
	Check if a publication is valid.

	@param pub The publication to check.

	@return HELICS_TRUE if the publication is a valid publication.
%}
function varargout = helicsPublicationIsValid(varargin)
	[varargout{1:nargout}] = helicsMex("helicsPublicationIsValid", varargin{:});
end
