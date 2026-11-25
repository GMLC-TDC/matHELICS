function varargout = helicsGetPropertyIndex(varargin)
% Get a property index for use in /ref helicsFederateInfoSetFlagOption, /ref helicsFederateInfoSetTimeProperty,
% or /ref helicsFederateInfoSetIntegerProperty
% @param val A string with the property name.
% @return An int with the property code, (-1) if val is NULL, or HELICS_INVALID_OPTION_INDEX if not a valid property.

	[varargout{1:nargout}] = helicsMex('helicsGetPropertyIndex', varargin{:});
end
