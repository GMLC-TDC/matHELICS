function varargout = helicsInputSetDefaultChar(varargin)
%  Set the default as a char.
% 
%	@param ipt The input to set the default for.
%	@param val The default char value.
% 
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultChar', varargin{:});
end
