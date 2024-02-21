function varargout = helicsDataBufferFillFromInteger(varargin)
% convert an integer to serialized bytes in a buffer
% @param data the HelicsDataBuffer to fill
% @param value the value to serialize into the dataBuffer
% @return the buffer size after filling

	[varargout{1:nargout}] = helicsMex('helicsDataBufferFillFromInteger', varargin{:});
end
