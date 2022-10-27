function varargout = helicsInputGetBoolean(varargin)
%  Get a boolean value from a subscription.
% 
%	@param ipt The input to get the data for.
% 
% 
%	@return A boolean value of current input value.
	[varargout{1:nargout}] = helicsMex('helicsInputGetBoolean', varargin{:});
end
