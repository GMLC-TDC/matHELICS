function varargout = helicsInputSetDefaultVector(varargin)
%	Set the default as a list of floats.

%	@param ipt The input to get the data for.
%	@param vectorInput The default list of floating point values.
	[varargout{1:nargout}] = helicsMex('helicsInputSetDefaultVector', varargin{:});
end
