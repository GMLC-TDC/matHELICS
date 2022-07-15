%{
	convert a HelicsBool to serialized bytes.

	@param value The HelicsBool.
	@return HelicsDataBuffer.
%}
function varargout = helicsBooleanToBytes(varargin)
	[varargout{1:nargout}] = helicsMex('helicsBooleanToBytes', varargin{:});
end
