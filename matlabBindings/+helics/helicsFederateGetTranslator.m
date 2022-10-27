function varargout = helicsFederateGetTranslator(varargin)
%  Get a translator by its name, typically already created via registerInterfaces file or something of that nature.
% 
%	@param fed The federate object to use to get the translator.
%	@param name The name of the translator.
% 
%	@param[in,out] err The error object to complete if there is an error.
%
% 
%	@return A HelicsTranslator object. If no translator with the specified name exists, the object will not be valid and
%	err will contain an error code.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetTranslator', varargin{:});
end
