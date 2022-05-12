%{
	Set the logging callback to a core

	@details Add a logging callback function to a core.
		The logging callback will be called when
		a message flows into a core from the core or from a broker.

	@param core The core object in which to set the callback.
	@param logger A function handle with the signature void(int loglevel, string identifier, string message).
%}
function varargout = helicsCoreSetLoggingCallback(varargin)
	[varargout{1:nargout}] = helicsMex("helicsCoreSetLoggingCallback", varargin{:});
end
