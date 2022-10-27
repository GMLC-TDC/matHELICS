function varargout = helicsTranslatorSetInfo(varargin)
%  Set the data in the info field for a translator.
% 
%	@param trans The given translator.
%	@param info The string to set.
% 

	[varargout{1:nargout}] = helicsMex('helicsTranslatorSetInfo', varargin{:});
end
