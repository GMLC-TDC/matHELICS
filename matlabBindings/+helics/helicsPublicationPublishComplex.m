function varargout = helicsPublicationPublishComplex(varargin)
%	Publish a complex number.

%	@param pub The publication to publish for.
%	@param value The complex number.
	[varargout{1:nargout}] = helicsMex('helicsPublicationPublishComplex', varargin{:});
end
