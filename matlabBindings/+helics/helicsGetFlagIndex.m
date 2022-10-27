function varargout = helicsGetFlagIndex(varargin)
 Get a property index for use in /ref helicsFederateInfoSetFlagOption, /ref helicsFederateSetFlagOption,
%	@param val A string with the option name.
%	@return An int with the property code or (-1) if not a valid property.
	[varargout{1:nargout}] = helicsMex('helicsGetFlagIndex', varargin{:});
end
