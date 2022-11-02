function varargout = helicsPublicationGetOption(varargin)
% Get the value of an option for a publication
% 
% @param pub The publication to query.
% @param option The value to query see /ref helics_handle_options.
% 
% @return A string with the info field string.

	[varargout{1:nargout}] = helicsMex('helicsPublicationGetOption', varargin{:});
end
