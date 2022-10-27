function varargout = helicsDataBufferToVector(varargin)
%	Get a vector from a HelicsDataBuffer.

%	@param data The HelicsDataBuffer to get the vector for.

%	@return  a list of floating point values.
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToVector', varargin{:});
end
