function varargout = helicsGetOptionIndex(varargin)
 Get an option index for use in /ref helicsPublicationSetOption, /ref helicsInputSetOption, /ref helicsEndpointSetOption,
%	/ref helicsFilterSetOption, and the corresponding get functions.

%	@param val A string with the option name.

%	@return An int with the option index or (-1) if not a valid property.
	[varargout{1:nargout}] = helicsMex('helicsGetOptionIndex', varargin{:});
end
