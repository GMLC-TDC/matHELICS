%{
	Publish a vector of doubles.

	@param pub The publication to publish for.
	@param vectorInput The list of floating point values.
%}
function varargout = helicsPublicationPublishVector(varargin)
	[varargout{1:nargout}] = helicsMex('helicsPublicationPublishVector', varargin{:});
end
