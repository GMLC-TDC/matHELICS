function varargout = helicsCreateMessageFederate(varargin)
%  Create a message federate from a federate info object.
% 
%	@details helics_message_federate objects can be used in all functions that take a helics_message_federate or HelicsFederate object as an
%	argument.
% 
%	@param fedName The name of the federate to create.
%	@param fi The federate info object that contains details on the federate.
% 
%
% 
%	@return An opaque message federate object.
	[varargout{1:nargout}] = helicsMex('helicsCreateMessageFederate', varargin{:});
end
