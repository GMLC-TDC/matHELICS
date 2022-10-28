function varargout = helicsPublicationSetOption(varargin)
%  Set the value of an option for a publication
% 
%	@param pub The publication to query.
%	@param option Integer code for the option to set /ref helics_handle_options.
%	@param val The value to set the option to.
% 
	[varargout{1:nargout}] = helicsMex('helicsPublicationSetOption', varargin{:});
end
