function varargout = helicsDataBufferFillFromBoolean(varargin)
% convert a bool to serialized bytesin a buffer
% @param data the HelicsDataBuffer to fill
% @param value the value to serialize into the dataBuffer
% @return the buffer size after filling

	[varargout{1:nargout}] = helicsMex('helicsDataBufferFillFromBoolean', varargin{:});
end
