function varargout = helicsDataBufferToComplex(varargin)
%	Get a complex value from an input object.

%	@param ipt The input to get the data for.

%	@return  A complex number.
	[varargout{1:nargout}] = helicsMex('helicsDataBufferToComplex', varargin{:});
end
