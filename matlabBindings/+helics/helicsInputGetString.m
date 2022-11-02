function varargout = helicsInputGetString(varargin)
%	Get a string value from a subscription.

%	@param ipt The input to get the string for.

%	@return the string value.
	[varargout{1:nargout}] = helicsMex('helicsInputGetString', varargin{:});
end
