function varargout = helicsDataBufferToRawString(varargin)
%	Get a raw string value from a HelicsDataBuffer.

%	@param data The HelicsDataBuffer to get the raw string from.

%	@return the raw string value.
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToRawString', varargin{:});
end
