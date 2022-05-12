%{
	Get the size of a value for subscription assuming return as an array of doubles.

	@return The number of doubles in a returned vector.
%}
function varargout = helicsInputGetVectorSize(varargin)
	[varargout{1:nargout}] = helicsMex("helicsInputGetVectorSize", varargin{:});
end
