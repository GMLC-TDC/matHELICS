function varargout = helicsCoreGetIdentifier(varargin)
%  Get an identifier for the core.
% 
%	@param core The core to query.
% 
%	@return A string with the identifier of the core.
	[varargout{1:nargout}] = helicsMex('helicsCoreGetIdentifier', varargin{:});
end
