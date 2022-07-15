%{
	Get a compelx vector from a HelicsDataBuffer.

	@param data The HelicsDataBuffer to get the vector for.

	@return a list of complex values.
%}
function varargout = helicsDataBufferToComplexVector(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToComplexVector', varargin{:});
end
