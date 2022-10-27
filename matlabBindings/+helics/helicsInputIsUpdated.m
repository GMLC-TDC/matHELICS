function varargout = helicsInputIsUpdated(varargin)
 Check if a particular subscription was updated.

%	@return HELICS_TRUE if it has been updated since the last value retrieval.
	[varargout{1:nargout}] = helicsMex('helicsInputIsUpdated', varargin{:});
end
