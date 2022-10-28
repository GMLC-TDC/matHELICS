function varargout = helicsFilterAddDeliveryEndpoint(varargin)
%  Add a delivery endpoint to a cloning filter.
% 
%	@details All cloned messages are sent to the delivery address(es).
% 
%	@param filt The given filter.
%	@param deliveryEndpoint The name of the endpoint to deliver messages to.
% 

	[varargout{1:nargout}] = helicsMex('helicsFilterAddDeliveryEndpoint', varargin{:});
end
