function varargout = helicsDataBufferToString(varargin)
%	Get a string value from a HelicsDataBuffer.

%	@param data The HelicsDataBuffer to get the string from.

%	@return the string value.
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToString', varargin{:});
end
