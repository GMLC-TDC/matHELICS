function varargout = helicsFederateGetEndpoint(varargin)
 Get an endpoint object from a name.

%	@param fed The message federate object to use to get the endpoint.
%	@param name The name of the endpoint.

%	@param[in,out] err The error object to complete if there is an error.


%	@return A HelicsEndpoint object.

%	The object will not be valid and err will contain an error code if no endpoint with the specified name exists.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetEndpoint', varargin{:});
end
