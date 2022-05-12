%{
	convert a HelicsTime to serialized bytes.

	@param value The HelicsTime value.
	@return HelicsDataBuffer.
%}
function varargout = helicsTimeToBytes(varargin)
	[varargout{1:nargout}] = helicsMex("helicsTimeToBytes", varargin{:});
end
