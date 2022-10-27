function varargout = helicsInputSetDefaultNamedPoint(varargin)
%  Set the default as a NamedPoint.
% 
%	@param ipt The input to set the default for.
%	@param defaultName A pointer to a null terminated string representing the field name to use in the named point.
%	@param val A double value for the value of the named point.
% 
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultNamedPoint', varargin{:});
end
