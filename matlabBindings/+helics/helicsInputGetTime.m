function varargout = helicsInputGetTime(varargin)
%  Get a time value from a subscription.
% 
%	@param ipt The input to get the data for.
% 
%	@param[in,out] err A pointer to an error object for catching errors.
% 
%	@return The resulting time value.
	[varargout{1:nargout}] = helicsMex('helicsInputGetTime', varargin{:});
end
