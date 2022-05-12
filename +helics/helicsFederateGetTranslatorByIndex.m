%{
	Get a translator by its index, typically already created via registerInterfaces file or something of that nature.

	@param fed The federate object in which to create a publication.
	@param index The index of the translator to get.

	@param[in,out] err A pointer to an error object for catching errors.


	@return A HelicsTranslator, which will be NULL if an invalid index is given.
%}
function varargout = helicsFederateGetTranslatorByIndex(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateGetTranslatorByIndex", varargin{:});
end
