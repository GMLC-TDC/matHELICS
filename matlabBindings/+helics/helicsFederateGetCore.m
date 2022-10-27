function varargout = helicsFederateGetCore(varargin)
 Get the core object associated with a federate.

%	@param fed A federate object.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.

%	@return A core object, nullptr if invalid.
	[varargout{1:nargout}] = helicsMex('helicsFederateGetCore', varargin{:});
end
