%{
%	Load interfaces from a file.

%	@param fed The federate to which to load interfaces.
%	@param file The name of a file to load the interfaces from either JSON, or TOML.

%	@param[in,out] err An error object that will contain an error code and string if any error occurred during the execution of the function.
%}
function varargout = helicsFederateRegisterInterfaces(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateRegisterInterfaces', varargin{:});
end
