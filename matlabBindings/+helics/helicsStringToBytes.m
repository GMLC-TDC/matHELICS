function varargout = helicsStringToBytes(varargin)
%	convert a string to serialized bytes.

%	@param str The string.
%	@return HelicsDataBuffer.
	[varargout{1:nargout}] = helicsMex('helicsStringToBytes', varargin{:});
end
