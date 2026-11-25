function varargout = helicsGetOptionIndex(varargin)
% Get an option index for use in /ref helicsPublicationSetOption, /ref helicsInputSetOption, /ref helicsEndpointSetOption,
% /ref helicsFilterSetOption, and the corresponding get functions.
% 
% @param val A string with the option name.
% 
% @return An int with the option index, (-1) if val is NULL, or HELICS_INVALID_OPTION_INDEX if not a valid option.

	[varargout{1:nargout}] = helicsMex('helicsGetOptionIndex', varargin{:});
end
