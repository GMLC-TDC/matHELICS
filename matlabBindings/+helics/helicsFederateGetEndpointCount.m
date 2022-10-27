function varargout = helicsFederateGetEndpointCount(varargin)
%  Get the number of endpoints in a federate.
% 
%	@param fed The message federate to query.
% 
%	@return (-1) if fed was not a valid federate, otherwise returns the number of endpoints.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetEndpointCount', varargin{:});
end
