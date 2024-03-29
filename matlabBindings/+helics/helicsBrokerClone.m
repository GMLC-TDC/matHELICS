function varargout = helicsBrokerClone(varargin)
% Create a new reference to an existing broker.
% 
% @details This will create a new broker object that references the existing broker it must be freed as well.
% 
% @param broker An existing HelicsBroker.
% 
% 
% 
% @return A new reference to the same broker.

	[varargout{1:nargout}] = helicsMex('helicsBrokerClone', varargin{:});
end
