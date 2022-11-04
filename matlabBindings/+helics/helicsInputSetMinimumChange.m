function varargout = helicsInputSetMinimumChange(varargin)
% Set the minimum change detection tolerance.
% 
% @param inp The input to modify.
% @param tolerance The tolerance level for registering an update, values changing less than this value will not show as being updated.
% 
% @param[in,out] err An error object to fill out in case of an error.

	[varargout{1:nargout}] = helicsMex('helicsInputSetMinimumChange', varargin{:});
end
