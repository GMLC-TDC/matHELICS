function varargout = helicsInputSetOption(varargin)
%  Set an option on an input
% 
%	@param inp The input to query.
%	@param option The option to set for the input /ref helics_handle_options.
%	@param value The value to set the option to.
% 
	[varargout{1:nargout}] = helicsMex('helicsInputSetOption', varargin{:});
end
