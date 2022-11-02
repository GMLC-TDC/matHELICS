function varargout = helicsInputGetOption(varargin)
% Get the current value of an input handle option
% 
% @param inp The input to query.
% @param option Integer representation of the option in question see /ref helics_handle_options.
% 
% @return An integer value with the current value of the given option.

	[varargout{1:nargout}] = helicsMex('helicsInputGetOption', varargin{:});
end
