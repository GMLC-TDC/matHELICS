%{ get the number of elements that would be required if a vector were retrieved*/%}
function varargout = helicsDataBufferVectorSize(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferVectorSize', varargin{:});
end
