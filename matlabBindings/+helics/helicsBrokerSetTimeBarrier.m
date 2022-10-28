function varargout = helicsBrokerSetTimeBarrier(varargin)
%  Set a broker time barrier.
% 
%	@param broker The broker to set the time barrier for.
%	@param barrierTime The time to set the barrier at.
% 
	[varargout{1:nargout}] = helicsMex('helicsBrokerSetTimeBarrier', varargin{:});
end
