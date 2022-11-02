function varargout = helicsPublicationPublishBytes(varargin)
%	Publish raw data.

%	@param pub The publication to publish for.
%	@param data the raw byte data to publish.
	[varargout{1:nargout}] = helicsMex('helicsPublicationPublishBytes', varargin{:});
end
