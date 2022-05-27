%{
	Complete the asynchronous disconnect/finalize call.  This call is identical to helicsFederateFinalizeComplete
%}
function varargout = helicsFederateDisconnectComplete(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateDisconnectComplete', varargin{:});
end
