%{
%	Remove a delivery destination from a cloning filter.

%	@param filt The given filter (must be a cloning filter).
%	@param deliveryEndpoint A string with the delivery endpoint to remove.

%	@param[in,out] err A pointer to an error object for catching errors.

%}
function varargout = helicsFilterRemoveDeliveryEndpoint(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFilterRemoveDeliveryEndpoint', varargin{:});
end
