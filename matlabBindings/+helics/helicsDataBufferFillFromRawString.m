function varargout = helicsDataBufferFillFromRawString(varargin)
% convert a raw string (may contain nulls) to serialized bytesin a buffer
% @param data the HelicsDataBuffer to fill
% @param str the value to serialize into the dataBuffer
% @param stringSize the size of the string value to serialize into the dataBuffer
% @return the buffer size after filling

	[varargout{1:nargout}] = helicsMex('helicsDataBufferFillFromRawString', varargin{:});
end
