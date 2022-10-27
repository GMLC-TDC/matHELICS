function varargout = helicsPublicationSetInfo(varargin)
%  Set the data in the info field for a publication.
% 
%	@param pub The publication to set the info field for.
%	@param info The string to set.
% 
	[varargout{1:nargout}] = helicsMex('helicsPublicationSetInfo', varargin{:});
end
