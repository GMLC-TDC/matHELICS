%{
	Disconnect/finalize the federate in an async call.  This call is identical to helicsFederateFinalizeAsync.
%}
function varargout = helicsFederateDisconnectAsync(varargin)
	[varargout{1:nargout}] = helicsMex("helicsFederateDisconnectAsync", varargin{:});
end
