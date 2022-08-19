%{
%	Set the logging callback for a federate

%	@details Add a logging callback function for a federate.
%		The logging callback will be called when
%		a message flows into a federate from the core or from a federate.

%	@param fed The federate object in which to set the callback.
%	@param logger A function handle with the signature void(int loglevel, string identifier, string message).
%}
function varargout = helicsFederateSetLoggingCallback(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateSetLoggingCallback', varargin{:});
end
