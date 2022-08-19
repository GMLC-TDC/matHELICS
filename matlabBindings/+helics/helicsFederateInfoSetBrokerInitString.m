%{
%	Set the initialization string that a core will pass to a generated broker usually in the form of command line arguments.

%	@param fi The federate info object to alter.
%	@param brokerInit A string with command line arguments for a generated broker.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

%}
function varargout = helicsFederateInfoSetBrokerInitString(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetBrokerInitString', varargin{:});
end
