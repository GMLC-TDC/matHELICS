function varargout = helicsCharToBytes(varargin)
%	convert a char to serialized bytes.

%	@param value The char.
%	@return HelicsDataBuffer.
	[varargout{1:nargout}] = helicsMex('helicsCharToBytes', varargin{:});
end
