function varargout = helicsTranslatorAddSourceEndpoint(varargin)
%  Add a source endpoint target to a translator.
% 
%	@details The translator will "translate" all message sent to it.  This method adds an endpoint which can send the translator data.
% 
%	@param trans The given translator.
%	@param ept The name of the endpoint which will send the endpoint data
% 

	[varargout{1:nargout}] = helicsMex('helicsTranslatorAddSourceEndpoint', varargin{:});
end
