%{
%	Set the default as a list of floats.

%	@param ipt The input to get the data for.
%	@param vectorInput The default list of complex values.
%}
function varargout = helicsInputSetDefaultComplexVector(varargin)
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultComplexVector', varargin{:});
end
