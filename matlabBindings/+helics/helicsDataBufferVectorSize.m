function varargout = helicsDataBufferVectorSize(varargin)
/** get the number of elements that would be required if a vector were retrieved*/
	[varargout{1:nargout}] = helicsMex('helicsDataBufferVectorSize', varargin{:});
end
