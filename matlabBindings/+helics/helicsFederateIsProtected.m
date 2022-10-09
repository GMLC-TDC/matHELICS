%{
	checks if an existing federate is protected


	@param fedName the name of an existing federate to check the protection status

	@param[in,out] err An error object that will contain an error code and string if the federate was not found.
%}
function varargout = helicsFederateIsProtected(varargin)
	[varargout{1:nargout}] = helicsMex('helicsFederateIsProtected', varargin{:});
end
