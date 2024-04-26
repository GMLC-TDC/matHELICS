function varargout = helicsDataBufferFillFromChar(varargin)
% convert a char to serialized bytesin a buffer
% @param data the HelicsDataBuffer to fill
% @param value the value to serialize into the dataBuffer
% @return the buffer size after filling

	[varargout{1:nargout}] = helicsMex('helicsDataBufferFillFromChar', varargin{:});
end
