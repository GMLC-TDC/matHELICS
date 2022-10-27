function varargout = helicsBrokerSetTimeBarrier(varargin)
 Set a broker time barrier.

%	@param broker The broker to set the time barrier for.
%	@param barrierTime The time to set the barrier at.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
	[varargout{1:nargout}] = helicsMex('helicsBrokerSetTimeBarrier', varargin{:});
end
