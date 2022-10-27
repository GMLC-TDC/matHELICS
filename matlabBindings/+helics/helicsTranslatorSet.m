function varargout = helicsTranslatorSet(varargin)
 Set a property on a translator.

%	@param trans The translator to modify.
%	@param prop A string containing the property to set.
%	@param val A numerical value for the property.

%	@param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsTranslatorSet', varargin{:});
end
