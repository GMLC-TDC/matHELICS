function varargout = helicsInputSetDefaultDouble(varargin)
%  Set the default as a double.
% 
%	@param ipt The input to set the default for.
%	@param val The default double value.
% 
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultDouble', varargin{:});
end
