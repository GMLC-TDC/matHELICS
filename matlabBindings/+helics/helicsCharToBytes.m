%{
%	convert a char to serialized bytes.

%	@param value The char.
%	@return HelicsDataBuffer.
%}
function varargout = helicsCharToBytes(varargin)
	[varargout{1:nargout}] = helicsMex('helicsCharToBytes', varargin{:});
end
