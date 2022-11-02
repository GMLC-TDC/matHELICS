function varargout = helicsFederateGetFilter(varargin)
% Get a filter by its name, typically already created via registerInterfaces file or something of that nature.
% 
% @param fed The federate object to use to get the filter.
% @param name The name of the filter.
% 
% 
% 
% @return A HelicsFilter object, the object will not be valid and err will contain an error code if no filter with the specified name
% exists.

	[varargout{1:nargout}] = helicsMex('helicsFederateGetFilter', varargin{:});
end
