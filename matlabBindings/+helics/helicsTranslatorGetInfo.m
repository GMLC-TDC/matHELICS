function varargout = helicsTranslatorGetInfo(varargin)
% Get the data in the info field of a translator.
% 
% @param trans The given translator.
% 
% @return A string with the info field string.

	[varargout{1:nargout}] = helicsMex('helicsTranslatorGetInfo', varargin{:});
end
