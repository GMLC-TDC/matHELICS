function varargout = helicsInputSetDefaultComplex(varargin)
%	Set the default as a complex number.

%	@param ipt The input to get the data for.
%	@param value The default complex value.
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultComplex', varargin{:});
end
