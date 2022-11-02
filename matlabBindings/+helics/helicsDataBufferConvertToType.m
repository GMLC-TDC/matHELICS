function varargout = helicsDataBufferConvertToType(varargin)
% convert the data in a data buffer to a different type representation
% @param data the buffer to convert
% @param newDataType the type that it is desired for the buffer to be converted to
% @return true if the conversion was successful

	[varargout{1:nargout}] = helicsMex('helicsDataBufferConvertToType', varargin{:});
end
