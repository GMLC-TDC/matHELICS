%{
%	convert a named point to serialized bytes.

%	@param name The name of the named point.
%	@param value The double value of the named point.
%	@return HelicsDataBuffer.
%}
function varargout = helicsNamedPointToBytes(varargin)
	[varargout{1:nargout}] = helicsMex('helicsNamedPointToBytes', varargin{:});
end
