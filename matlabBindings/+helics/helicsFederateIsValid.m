function varargout = helicsFederateIsValid(varargin)
% Check if a federate_object is valid.
% 
% @return HELICS_TRUE if the federate is a valid active federate, HELICS_FALSE otherwise

	[varargout{1:nargout}] = helicsMex('helicsFederateIsValid', varargin{:});
end
