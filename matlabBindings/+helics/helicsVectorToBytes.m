function varargout = helicsVectorToBytes(varargin)
%	convert a real vector to serialized bytes.

%	@param value The vector of doubles.
%	@return HelicsDataBuffer.
	[varargout{1:nargout}] = helicsMex('helicsVectorToBytes', varargin{:});
end
