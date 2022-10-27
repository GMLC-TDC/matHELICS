function varargout = helicsPublicationSetInfo(varargin)
 Set the data in the info field for a publication.

%	@param pub The publication to set the info field for.
%	@param info The string to set.

%	@param[in,out] err An error object to fill out in case of an error.
	[varargout{1:nargout}] = helicsMex('helicsPublicationSetInfo', varargin{:});
end
