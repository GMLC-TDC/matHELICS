function varargout = helicsFederateEnterInitializingMode(varargin)
%  Enter the initialization state of a federate.
% 
%	@details The initialization state allows initial values to be set and received if the iteration is requested on entry to the execution
%	state. This is a blocking call and will block until the core allows it to proceed.
% 
%	@param fed The federate to operate on.
% 
	[varargout{1:nargout}] = helicsMex('helicsFederateEnterInitializingMode', varargin{:});
end
