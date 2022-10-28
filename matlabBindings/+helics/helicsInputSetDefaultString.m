function varargout = helicsInputSetDefaultString(varargin)
%  Set the default as a string.
% 
%	@param ipt The input to set the default for.
%	@param defaultString A pointer to the default string.
% 
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultString', varargin{:});
end
