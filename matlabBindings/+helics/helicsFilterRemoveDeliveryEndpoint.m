function varargout = helicsFilterRemoveDeliveryEndpoint(varargin)
 Remove a delivery destination from a cloning filter.

%	@param filt The given filter (must be a cloning filter).
%	@param deliveryEndpoint A string with the delivery endpoint to remove.

%	@param[in,out] err A pointer to an error object for catching errors.

	[varargout{1:nargout}] = helicsMex('helicsFilterRemoveDeliveryEndpoint', varargin{:});
end
