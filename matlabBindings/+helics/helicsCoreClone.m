function varargout = helicsCoreClone(varargin)
% Create a new reference to an existing core.
% 
% @details This will create a new broker object that references the existing broker. The new broker object must be freed as well.
% 
% @param core An existing HelicsCore.
% 
% @return A new reference to the same broker.

	[varargout{1:nargout}] = helicsMex('helicsCoreClone', varargin{:});
end
