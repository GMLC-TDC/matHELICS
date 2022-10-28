function varargout = helicsTranslatorSetString(varargin)
%  Set a string property on a translator.
% 
%	@param trans The translator to modify.
%	@param prop A string containing the property to set.
%	@param val A string containing the new value.
% 

	[varargout{1:nargout}] = helicsMex('helicsTranslatorSetString', varargin{:});
end
