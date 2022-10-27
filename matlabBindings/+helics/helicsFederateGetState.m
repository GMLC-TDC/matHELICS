function varargout = helicsFederateGetState(varargin)
 Get the current state of a federate.

%	@param fed The federate to query.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%	The err object will be removed in a future release as it is not necessary for use the function will not error, invalid federate return
%	HELICS_STATE_UNKOWN

%	@return State the resulting state if the federate is invalid will return HELICS_STATE_UNKNOWN
	[varargout{1:nargout}] = helicsMex('helicsFederateGetState', varargin{:});
end
