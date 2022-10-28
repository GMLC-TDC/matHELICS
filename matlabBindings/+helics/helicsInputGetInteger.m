function varargout = helicsInputGetInteger(varargin)
%  Get an integer value from a subscription.
% 
%	@param ipt The input to get the data for.
% 
% 
%	@return An int64_t value with the current value of the input.
	[varargout{1:nargout}] = helicsMex('helicsInputGetInteger', varargin{:});
end
