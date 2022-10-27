function varargout = helicsEndpointCreateMessage(varargin)
%  Create a new empty message object.
% 
%	@details The message is empty and isValid will return false since there is no data associated with the message yet.
% 
%	@param endpoint The endpoint object to associate the message with.
% 
%	@param[in,out] err An error object to fill out in case of an error.
%
% 
%	@return A new HelicsMessage.
	[varargout{1:nargout}] = helicsMex('helicsEndpointCreateMessage', varargin{:});
end
