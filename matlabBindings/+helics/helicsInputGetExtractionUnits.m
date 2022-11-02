function varargout = helicsInputGetExtractionUnits(varargin)
% Get the units of an input.
% 
% @details The same as helicsInputGetUnits.
% 
% @param ipt The input to query.
% 
% @return A void enumeration, HELICS_OK if everything worked.

	[varargout{1:nargout}] = helicsMex('helicsInputGetExtractionUnits', varargin{:});
end
