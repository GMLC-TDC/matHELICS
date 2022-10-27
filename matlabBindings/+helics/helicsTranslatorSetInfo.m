function varargout = helicsTranslatorSetInfo(varargin)
%  Set the data in the info field for a translator.
% 
%	@param trans The given translator.
%	@param info The string to set.
% 
%	@param[in,out] err An error object to fill out in case of an error.

	[varargout{1:nargout}] = helicsMex('helicsTranslatorSetInfo', varargin{:});
end
