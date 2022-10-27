function varargout = helicsPublicationGetUnits(varargin)
 Get the units of a publication.

%	@param pub The publication to query.

%	@return A void enumeration, HELICS_OK if everything worked.
	[varargout{1:nargout}] = helicsMex('helicsPublicationGetUnits', varargin{:});
end
