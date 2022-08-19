%{
%	Complete the asynchronous disconnect/finalize call.
%}
function varargout = helicsFederateFinalizeComplete(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateFinalizeComplete', varargin{:});
end
