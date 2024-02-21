function varargout = helicsPublicationPublishDataBuffer(varargin)
% Publish the contents of a helicsDataBuffer.
% 
% @param pub The publication to publish for.
% @param buffer a HelicsDataBuffer object containing the data to publish
% 
% @param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsPublicationPublishDataBuffer', varargin{:});
end
