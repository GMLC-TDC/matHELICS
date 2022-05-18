%{ Execute a global abort by sending an error code to all cores, brokers,
and federates that were created through the current library instance.*/%}
function varargout = helicsAbort(varargin)
	[varargout{1:nargout}] = helicsMex('helicsAbort', varargin{:});
end
