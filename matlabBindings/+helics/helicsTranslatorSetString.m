%{
%	Set a string property on a translator.

%	@param trans The translator to modify.
%	@param prop A string containing the property to set.
%	@param val A string containing the new value.

%	@param[in,out] err A pointer to an error object for catching errors.

%}
function varargout = helicsTranslatorSetString(varargin)
	[varargout{1:nargout}] = helicsMex('helicsTranslatorSetString', varargin{:});
end
