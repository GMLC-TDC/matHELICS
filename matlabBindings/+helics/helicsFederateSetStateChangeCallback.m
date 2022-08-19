%{
%	Set callback for the federate mode change.n
%	@details This callback will be executed every time the operating mode of the federate changes.

%	@param fed The federate object in which to set the callback.
%	@param stateChange A function handle with the signature void(int newState, int oldState).
%}
function varargout = helicsFederateSetStateChangeCallback(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateSetStateChangeCallback', varargin{:});
end
