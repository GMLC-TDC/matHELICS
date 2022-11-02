function varargout = helicsGetOptionValue(varargin)
% Get an option value for use in /ref helicsPublicationSetOption, /ref helicsInputSetOption, /ref helicsEndpointSetOption,
% /ref helicsFilterSetOption.
% 
% @param val A string representing the value.
% 
% @return An int with the option value or (-1) if not a valid value.

	[varargout{1:nargout}] = helicsMex('helicsGetOptionValue', varargin{:});
end
