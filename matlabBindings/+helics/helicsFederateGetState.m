function varargout = helicsFederateGetState(varargin)
% Get the current state of a federate.
% 
% @param fed The federate to query.
% 
% The err object will be removed in a future release as it is not necessary for use the function will not error, invalid federate return
% HELICS_STATE_UNKOWN
% 
% @return State the resulting state if the federate is invalid will return HELICS_STATE_UNKNOWN

	[varargout{1:nargout}] = helicsMex('helicsFederateGetState', varargin{:});
end
