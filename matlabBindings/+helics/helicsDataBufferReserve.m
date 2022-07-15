%{ increase the capacity a data buffer can hold without reallocating memory
@return HELICS_TRUE if the reservation was successful HELICS_FALSE otherwise*/%}
function varargout = helicsDataBufferReserve(varargin)
	[varargout{1:nargout}] = helicsMex('helicsDataBufferReserve', varargin{:});
end
