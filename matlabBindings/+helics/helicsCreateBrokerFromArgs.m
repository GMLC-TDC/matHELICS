%{
	Create a broker object by passing command line arguments.

	@param type The type of the core to create.
	@param name The name of the core.
	@param arguments The list of string values from a command line.

	@return A HelicsBroker object.
%}
function varargout = helicsCreateBrokerFromArgs(varargin)
	[varargout{1:nargout}] = helicsMex('helicsCreateBrokerFromArgs', varargin{:});
end
