%{
%	Set callback for the federate time update.n
%	@details This callback will be executed every time the simulation time is updated starting on entry to executing mode.

%	@param fed The federate object in which to set the callback.
%	@param timeUpdate A function handle with the signature void(double newTime, int iterating).
%}
function varargout = helicsFederateSetTimeUpdateCallback(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateSetTimeUpdateCallback', varargin{:});
end
