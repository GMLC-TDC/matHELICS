%{
%	Set the value of an option for a publication

%	@param pub The publication to query.
%	@param option Integer code for the option to set /ref helics_handle_options.
%	@param val The value to set the option to.

%	@param[in,out] err An error object to fill out in case of an error.
%}
function varargout = helicsPublicationSetOption(varargin)
	[varargout{1:nargout}] = helicsMex('helicsPublicationSetOption', varargin{:});
end
