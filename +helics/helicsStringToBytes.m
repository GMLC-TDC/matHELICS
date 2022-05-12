%{
	convert a string to serialized bytes.

	@param str The string.
	@return HelicsDataBuffer.
%}
function varargout = helicsStringToBytes(varargin)
	[varargout{1:nargout}] = helicsMex("helicsStringToBytes", varargin{:});
end
