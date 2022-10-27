function varargout = helicsFederateDisconnectComplete(varargin)
 Complete the asynchronous disconnect/finalize call.  This call is identical to helicsFederateFinalizeComplete
	[varargout{1:nargout}] = helicsMex('helicsFederateDisconnectComplete', varargin{:});
end
