function varargout = helicsInputSetDefaultBytes(varargin)
%	Set the default as a raw data array.

%	@param ipt The input to set the default for.
%	@param raw data to use for the default.
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultBytes', varargin{:});
end
