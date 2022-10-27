function varargout = helicsTranslatorSetOption(varargin)
%  Set an option value for a translator.
% 
%	@param trans The given translator.
%	@param option The option to set /ref helics_handle_options.
%	@param value The value of the option, commonly 0 for false or 1 for true.
% 
%	@param[in,out] err An error object to fill out in case of an error.

	[varargout{1:nargout}] = helicsMex('helicsTranslatorSetOption', varargin{:});
end
