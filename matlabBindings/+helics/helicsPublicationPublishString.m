function varargout = helicsPublicationPublishString(varargin)
%  Publish a string.
% 
%	@param pub The publication to publish for.
%	@param val The null terminated string to publish.
% 

	[varargout{1:nargout}] = helicsMex('helicsPublicationPublishString', varargin{:});
end
