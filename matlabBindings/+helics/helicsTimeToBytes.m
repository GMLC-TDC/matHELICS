function varargout = helicsTimeToBytes(varargin)
%	convert a HelicsTime to serialized bytes.

%	@param value The HelicsTime value.
%	@return HelicsDataBuffer.
	[varargout{1:nargout}] = helicsMex('helicsTimeToBytes', varargin{:});
end
