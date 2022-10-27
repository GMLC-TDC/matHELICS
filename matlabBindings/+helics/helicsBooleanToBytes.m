function varargout = helicsBooleanToBytes(varargin)
%	convert a HelicsBool to serialized bytes.

%	@param value The HelicsBool.
%	@return HelicsDataBuffer.
	[varargout{1:nargout}] = helicsMex('helicsBooleanToBytes', varargin{:});
end
