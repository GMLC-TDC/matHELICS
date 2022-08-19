%{
%	Get a vector from a HelicsDataBuffer.

%	@param data The HelicsDataBuffer to get the vector for.

%	@return  a list of floating point values.
%}
function varargout = helicsDataBufferToVector(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToVector', varargin{:});
end
