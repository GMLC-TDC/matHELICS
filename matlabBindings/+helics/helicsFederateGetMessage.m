function varargout = helicsFederateGetMessage(varargin)
 Receive a communication message for any endpoint in the federate.

%	@details The return order will be in order of endpoint creation.
%	         So all messages that are available for the first endpoint, then all for the second, and so on.
%	         Within a single endpoint, the messages are ordered by time, then source_id, then order of arrival.

%	@return A HelicsMessage which references the data in the message.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetMessage', varargin{:});
end
