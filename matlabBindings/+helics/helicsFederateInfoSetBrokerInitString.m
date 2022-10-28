function varargout = helicsFederateInfoSetBrokerInitString(varargin)
%  Set the initialization string that a core will pass to a generated broker usually in the form of command line arguments.
% 
%	@param fi The federate info object to alter.
%	@param brokerInit A string with command line arguments for a generated broker.
% 

	[varargout{1:nargout}] = helicsMex('helicsFederateInfoSetBrokerInitString', varargin{:});
end
