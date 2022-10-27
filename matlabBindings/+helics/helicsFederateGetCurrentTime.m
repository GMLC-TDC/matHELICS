function varargout = helicsFederateGetCurrentTime(varargin)
 Get the current time of the federate.

%	@param fed The federate object to query.

%	@param[in,out] err A pointer to an error object for catching errors.

%	@return The current time of the federate.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetCurrentTime', varargin{:});
end
