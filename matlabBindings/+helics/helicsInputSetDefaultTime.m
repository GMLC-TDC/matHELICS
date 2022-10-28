function varargout = helicsInputSetDefaultTime(varargin)
%  Set the default as a time.
% 
%	@param ipt The input to set the default for.
%	@param val The default time value.
% 
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultTime', varargin{:});
end
