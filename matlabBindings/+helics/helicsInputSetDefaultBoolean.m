function varargout = helicsInputSetDefaultBoolean(varargin)
%  Set the default as a boolean.
% 
%	@param ipt The input to set the default for.
%	@param val The default boolean value.
% 
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultBoolean', varargin{:});
end
