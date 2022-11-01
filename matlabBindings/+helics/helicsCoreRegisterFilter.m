function varargout = helicsCoreRegisterFilter(varargin)
% Create a source Filter on the specified core.
% 
% @details Filters can be created through a federate or a core, linking through a federate allows
%          a few extra features of name matching to function on the federate interface but otherwise equivalent behavior.
% 
% @param core The core to register through.
% @param type The type of filter to create /ref HelicsFilterTypes.
% @param name The name of the filter (can be NULL).
% 
% 
% 
% @return A HelicsFilter object.

	[varargout{1:nargout}] = helicsMex('helicsCoreRegisterFilter', varargin{:});
end
