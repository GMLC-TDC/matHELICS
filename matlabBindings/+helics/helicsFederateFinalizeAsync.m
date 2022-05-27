%{
	Disconnect/finalize the federate in an async call.
%}
function varargout = helicsFederateFinalizeAsync(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateFinalizeAsync', varargin{:});
end
