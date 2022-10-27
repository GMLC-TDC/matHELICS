function varargout = helicsTranslatorSetTag(varargin)
%  Set the data in a specific tag for a translator.
% 
%	@param trans The translator object to set the tag for.
%	@param tagname The string to set.
%	@param tagvalue The string value to associate with a tag.
% 
%	@param[in,out] err An error object to fill out in case of an error.

	[varargout{1:nargout}] = helicsMex('helicsTranslatorSetTag', varargin{:});
end
