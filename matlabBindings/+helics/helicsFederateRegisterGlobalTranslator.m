function varargout = helicsFederateRegisterGlobalTranslator(varargin)
%  Create a global translator through a federate.
% 
%	@details Translators can be created through a federate or a core. Linking through a federate allows
%	         a few extra features of name matching to function on the federate interface but otherwise have equivalent behavior.
% 
%	@param fed The federate to register through.
%	@param type The type of translator to create /ref HelicsTranslatorTypes.
%	@param name The name of the translator (can be NULL).
% 
%	@param[in,out] err A pointer to an error object for catching errors.
%
% 
%	@return A HelicsTranslator object.
	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterGlobalTranslator', varargin{:});
end
