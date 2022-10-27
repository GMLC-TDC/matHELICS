function varargout = helicsPublicationPublishComplexVector(varargin)
%	Publish a vector of doubles.

%	@param pub The publication to publish for.
%	@param vectorInput The list of complex values.
	[varargout{1:nargout}] = helicsMex('helicsPublicationPublishComplexVector', varargin{:});
end
