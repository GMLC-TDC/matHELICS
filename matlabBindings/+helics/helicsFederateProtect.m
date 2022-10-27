function varargout = helicsFederateProtect(varargin)
 Protect a federate from finalizing and closing if all references go out of scope

%	@details this function allows a federate to be retrieved on demand, it must be explicitly close later otherwise it will be destroyed
%	when the library is closed

%	@param fedName The name of an existing HelicsFederate.

%	@param[in,out] err An error object that will contain an error code and string if any error
 occurred during the execution of the function, in particular if no federate with the given name exists
	[varargout{1:nargout}] = helicsMex('helicsFederateProtect', varargin{:});
end
