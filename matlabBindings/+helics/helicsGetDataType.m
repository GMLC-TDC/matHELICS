function varargout = helicsGetDataType(varargin)
% Get the data type for use in /ref helicsFederateRegisterPublication, /ref helicsFederateRegisterInput,
% /ref helicsFilterSetOption.
% 
% @param val A string representing a data type.
% 
% @return An int with the data type or HELICS_DATA_TYPE_UNKNOWN(-1) if not a valid value.

	[varargout{1:nargout}] = helicsMex('helicsGetDataType', varargin{:});
end
