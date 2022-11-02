function varargout = helicsDataBufferToComplexVector(varargin)
%	Get a compelx vector from a HelicsDataBuffer.

%	@param data The HelicsDataBuffer to get the vector for.

%	@return a list of complex values.
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToComplexVector', varargin{:});
end
