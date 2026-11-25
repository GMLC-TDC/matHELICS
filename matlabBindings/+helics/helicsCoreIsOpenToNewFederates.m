function varargout = helicsCoreIsOpenToNewFederates(varargin)
% Check if a core can accept new federates.
% 
% @param core The core to query.
% 
% @return HELICS_TRUE if the core is open to new federates, HELICS_FALSE otherwise.

	[varargout{1:nargout}] = helicsMex('helicsCoreIsOpenToNewFederates', varargin{:});
end
