%{
%	Publish a complex number.

%	@param pub The publication to publish for.
%	@param value The complex number.
%}
function varargout = helicsPublicationPublishComplex(varargin)
	[varargout{1:nargout}] = helicsMex('helicsPublicationPublishComplex', varargin{:});
end
