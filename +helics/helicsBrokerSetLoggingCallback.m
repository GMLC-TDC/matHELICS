%{
	Set the logging callback to a broker

	@details Add a logging callback function to a broker.
		The logging callback will be called when
		a message flows into a broker from the core or from a broker.

	@param broker The broker object in which to set the callback.
	@param logger A function handle with the signature void(int loglevel, string identifier, string message).
%}
function varargout = helicsBrokerSetLoggingCallback(varargin)
	[varargout{1:nargout}] = helicsMex("helicsBrokerSetLoggingCallback", varargin{:});
end
