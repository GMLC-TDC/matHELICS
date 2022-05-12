%{
	convert a HelicsBool to serialized bytes.

	@param value The HelicsBool.
	@return HelicsDataBuffer.
%}
function varargout = helicsBoolToBytes(varargin)
	[varargout{1:nargout}] = helicsMex("helicsBoolToBytes", varargin{:});
end
