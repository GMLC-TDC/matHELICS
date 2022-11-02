function varargout = helicsFederateCreateMessage(varargin)
% Create a new empty message object.
% 
% @details The message is empty and isValid will return false since there is no data associated with the message yet.
% 
% @param fed the federate object to associate the message with
% 
% 
% 
% @return A HelicsMessage containing the message data.

	[varargout{1:nargout}] = helicsMex('helicsFederateCreateMessage', varargin{:});
end
