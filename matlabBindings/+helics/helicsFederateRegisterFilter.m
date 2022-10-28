function varargout = helicsFederateRegisterFilter(varargin)
%  Create a source Filter on the specified federate.
% 
%	@details Filters can be created through a federate or a core, linking through a federate allows
%	         a few extra features of name matching to function on the federate interface but otherwise equivalent behavior
% 
%	@param fed The federate to register through.
%	@param type The type of filter to create /ref HelicsFilterTypes.
%	@param name The name of the filter (can be NULL).
% 
%
% 
%	@return A HelicsFilter object.
	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterFilter', varargin{:});
end
