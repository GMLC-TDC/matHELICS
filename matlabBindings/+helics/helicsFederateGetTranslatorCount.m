function varargout = helicsFederateGetTranslatorCount(varargin)
%  Get the number of translators registered through a federate.
% 
%	@param fed The federate object to use to get the translator.
% 
%	@return A count of the number of translators registered through a federate.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetTranslatorCount', varargin{:});
end
