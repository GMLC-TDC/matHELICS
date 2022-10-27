function varargout = helicsFederateRegisterGlobalEndpoint(varargin)
%  Create an endpoint.
% 
%	@details The endpoint becomes part of the federate and is destroyed when the federate is freed
%	         so there are no separate free functions for endpoints.
% 
%	@param fed The federate object in which to create an endpoint must have been created
%             with helicsCreateMessageFederate or helicsCreateCombinationFederate.
%	@param name The identifier for the endpoint, the given name is the global identifier.
%	@param type A string describing the expected type of the publication (may be NULL).
% 
%
%	@return An object containing the endpoint, or nullptr on failure.
	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterGlobalEndpoint', varargin{:});
end
