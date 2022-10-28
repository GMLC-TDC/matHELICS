function varargout = helicsFederateGetEndpointByIndex(varargin)
%  Get an endpoint by its index, typically already created via registerInterfaces file or something of that nature.
% 
%	@param fed The federate object in which to create a publication.
%	@param index The index of the publication to get.
% 
%
% 
%	@return A HelicsEndpoint.
% 
%	The HelicsEndpoint returned will be NULL if given an invalid index.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetEndpointByIndex', varargin{:});
end
