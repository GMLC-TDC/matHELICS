function varargout = helicsFederateDisconnectAsync(varargin)
% Disconnect/finalize the federate in an async call.  This call is identical to helicsFederateFinalizeAsync.

	[varargout{1:nargout}] = helicsMex('helicsFederateDisconnectAsync', varargin{:});
end
