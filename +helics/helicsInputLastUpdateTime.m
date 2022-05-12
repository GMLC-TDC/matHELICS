%{
	Get the last time a subscription was updated.
%}
function varargout = helicsInputLastUpdateTime(varargin)
	[varargout{1:nargout}] = helicsMex("helicsInputLastUpdateTime", varargin{:});
end
