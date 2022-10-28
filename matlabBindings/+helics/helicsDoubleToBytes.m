function varargout = helicsDoubleToBytes(varargin)
%	convert a double to serialized bytes.

%	@param value The double.
%	@return HelicsDataBuffer.
	[varargout{1:nargout}] = helicsMex('helicsDoubleToBytes', varargin{:});
end
