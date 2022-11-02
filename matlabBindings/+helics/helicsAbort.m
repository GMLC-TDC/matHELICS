function varargout = helicsAbort(varargin)
% Execute a global abort by sending an error code to all cores, brokers,
% and federates that were created through the current library instance.

	[varargout{1:nargout}] = helicsMex('helicsAbort', varargin{:});
end
