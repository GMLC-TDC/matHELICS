function varargout = helicsInputSetDefaultInteger(varargin)
%  Set the default as an integer.
% 
%	@param ipt The input to set the default for.
%	@param val The default integer.
% 
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultInteger', varargin{:});
end
