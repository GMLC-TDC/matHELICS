function varargout = helicsFederateGetFilterCount(varargin)
% Get the number of filters registered through a federate.
% 
% @param fed The federate object to use to get the filter.
% 
% @return A count of the number of filters registered through a federate.

	[varargout{1:nargout}] = helicsMex('helicsFederateGetFilterCount', varargin{:});
end
