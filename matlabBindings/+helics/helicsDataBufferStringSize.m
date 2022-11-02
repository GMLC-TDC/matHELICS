function varargout = helicsDataBufferStringSize(varargin)
% get the size of memory required to retrieve a string from a data buffer this includes space for a null terminator

	[varargout{1:nargout}] = helicsMex('helicsDataBufferStringSize', varargin{:});
end
