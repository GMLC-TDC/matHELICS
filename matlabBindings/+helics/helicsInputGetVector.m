function varargout = helicsInputGetVector(varargin)
%	Get a vector from a subscription.

%	@param ipt The input to get the vector for.

%	@return  a list of floating point values.
	[varargout{1:nargout}] = helicsMex('helicsInputGetVector', varargin{:});
end
