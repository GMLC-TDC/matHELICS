function varargout = helicsFederateAddDependency(varargin)
 Add a time dependency for a federate. The federate will depend on the given named federate for time synchronization.

%	@param fed The federate to add the dependency for.
%	@param fedName The name of the federate to depend on.

%	@param[in,out] err A pointer to an error object for catching errors.
	[varargout{1:nargout}] = helicsMex('helicsFederateAddDependency', varargin{:});
end
