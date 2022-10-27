function varargout = helicsFederateClearMessages(varargin)
%  Clear all stored messages from a federate.
% 
%	@details This clears messages retrieved through helicsEndpointGetMessage or helicsFederateGetMessage
% 
%	@param fed The federate to clear the message for.
	[varargout{1:nargout}] = helicsMex('helicsFederateClearMessages', varargin{:});
end
