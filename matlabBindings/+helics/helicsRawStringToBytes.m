function varargout = helicsRawStringToBytes(varargin)
%	convert a raw string to serialized bytes.

%	@param str The string.
%	@return HelicsDataBuffer.
	[varargout{1:nargout}] = helicsMex('helicsRawStringToBytes', varargin{:});
end
