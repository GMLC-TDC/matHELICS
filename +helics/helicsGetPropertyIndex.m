%{
	Get a property index for use in /ref helicsFederateInfoSetFlagOption, /ref helicsFederateInfoSetTimeProperty,
	or /ref helicsFederateInfoSetIntegerProperty
	@param val A string with the property name.
	@return An int with the property code or (-1) if not a valid property.
%}
function varargout = helicsGetPropertyIndex(varargin)
	[varargout{1:nargout}] = helicsMex("helicsGetPropertyIndex", varargin{:});
end
