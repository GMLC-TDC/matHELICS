%{
	Get a complex value from an HelicsDataBuffer.

	@param data The HelicsDataBuffer to get the data for.

	@return  A complex number.
%}
function varargout = helicsDataBufferToComplexObject(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToComplexObject', varargin{:});
end
