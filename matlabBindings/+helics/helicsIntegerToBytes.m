function varargout = helicsIntegerToBytes(varargin)
%	convert an int to serialized bytes.

%	@param value The integer.
%	@return HelicsDataBuffer.
	[varargout{1:nargout}] = helicsMex('helicsIntegerToBytes', varargin{:});
end
