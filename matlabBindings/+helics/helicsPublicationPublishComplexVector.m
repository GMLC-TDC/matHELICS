%{
%	Publish a vector of doubles.

%	@param pub The publication to publish for.
%	@param vectorInput The list of complex values.
%}
function varargout = helicsPublicationPublishComplexVector(varargin)
	[varargout{1:nargout}] = helicsMex('helicsPublicationPublishComplexVector', varargin{:});
end
