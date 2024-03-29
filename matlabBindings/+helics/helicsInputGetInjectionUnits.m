function varargout = helicsInputGetInjectionUnits(varargin)
% Get the units of the publication that an input is linked to.
% 
% @param ipt The input to query.
% 
% @return A void enumeration, HELICS_OK if everything worked.

	[varargout{1:nargout}] = helicsMex('helicsInputGetInjectionUnits', varargin{:});
end
