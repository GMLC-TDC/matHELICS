function varargout = helicsFederateGetSubscription(varargin)
% Get an input object from a subscription target.
% 
% @param fed The value federate object to use to get the publication.
% @param key The name of the publication that a subscription is targeting.
% 
% 
% 
% @return A HelicsInput object, the object will not be valid and err will contain an error code if no input with the specified
% key exists.

	[varargout{1:nargout}] = helicsMex('helicsFederateGetSubscription', varargin{:});
end
