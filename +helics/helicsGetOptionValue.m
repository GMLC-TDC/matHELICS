%{
	Get an option value for use in /ref helicsPublicationSetOption, /ref helicsInputSetOption, /ref helicsEndpointSetOption,
	/ref helicsFilterSetOption.

	@param val A string representing the value.

	@return An int with the option value or (-1) if not a valid value.
%}
function varargout = helicsGetOptionValue(varargin)
	[varargout{1:nargout}] = helicsMex("helicsGetOptionValue", varargin{:});
end
